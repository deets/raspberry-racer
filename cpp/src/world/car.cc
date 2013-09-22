#include <cassert>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#include "world/car.hh"

namespace rracer {

  Car::Car(AssetManager& am, const Json::Value& car_info) 
    : _am(am)
    , _accelerate(false)
  {
    assert(car_info.isMember("image") && car_info["image"].isString());
    assert(car_info.isMember("length") && car_info["length"].isDouble());
    assert(car_info.isMember("width") && car_info["width"].isDouble());
    assert(car_info.isMember("mass") && car_info["mass"].isDouble());
    assert(car_info.isMember("slot-offset") && car_info["slot-offset"].isDouble());
    assert(car_info.isMember("wheels") && car_info["wheels"].isArray());
    assert(car_info.isMember("power") && car_info["power"].isDouble());
    _engine.power = car_info["power"].asDouble();
    _image_name = car_info["image"].asString();
    _length = car_info["length"].asDouble();
    _width = car_info["width"].asDouble();
    _mass = car_info["mass"].asDouble();
    _slot_offset = car_info["slot-offset"].asDouble();
    Json::Value json_wheels = car_info["wheels"];
    for(int i = 0; i < json_wheels.size(); ++i) {
      Json::Value json_wheel = json_wheels[i];
      _wheels.push_back(Wheel(json_wheel));
    }
  }


  Car::~Car() {
    BOOST_REVERSE_FOREACH(function< void ()> destroy, _destroyers) {
      destroy();
    }
    _body = 0;
  }


  void Car::render(OpenVGCompanion& vgc) const {
    ConnectionPoint position = Car::position();
    const PNGImageData& img_data = _am.image(_image_name);
    // move to the middle axis of the car
    AffineTransform t = translate(Vector(-img_data.width() / 2.0, -img_data.height() / 2.0));
    // scale to world-size
    t = scale(_length / img_data.height()) * t;
    // 0 degrees shows to the right,
    // so adjust for the fact that
    // our cars are drawn facing front to upwards
    t = rotation(-90) * t;
    // rot/t to our position
    t = rotation(position.direction) * t;
    t = translate(position.point) * t;
    MatrixStacker(vgc, t);
    PaintScope(vgc, Color::black, VG_FILL_PATH | VG_STROKE_PATH);
    vgc.drawImage(img_data);
  }


  void Car::step(Real elapsed) {
    BOOST_FOREACH(Wheel& wheel, _wheels) {
      wheel.step(elapsed, _accelerate, _engine);
    }
  }

  void Car::process_input_events(const InputEventVector& events, double elapsed) {
    BOOST_FOREACH(const InputEvent event, events) {
      switch(event.key) {
      case K_UP:
	_accelerate = event.pressed;
      }
    }
    this->step(elapsed);
  }


  void Car::physics_setup(b2World* world) {
    _world = world;
    b2BodyDef body_def;
    body_def.type = b2_dynamicBody;
    body_def.linearDamping = 0.0f;
    body_def.angularDamping = CAR_ANGULAR_DAMPING;
    body_def.active = true;
    body_def.position.Set(0.0f, 0.0f);
    body_def.angle = 0.0f;
    _body = world->CreateBody(&body_def);
    _destroyers.push_back(bind(&b2World::DestroyBody, _world, _body));

    b2PolygonShape chassis;
    chassis.SetAsBox(_length / 2.0, _width / 2.0); // box2d uses half-widths here
    b2FixtureDef fixture_def;
    fixture_def.shape = &chassis;
    fixture_def.friction = 0.0f;
    fixture_def.density = _mass / (_length * _width);
    _body->CreateFixture(&fixture_def);
    BOOST_FOREACH(Wheel& wheel, _wheels) {
      wheel.physics_setup(world, _body, _destroyers);
    }
  }


  ConnectionPoint Car::position() const {
    ConnectionPoint res;
    b2Vec2 center = _body->GetWorldCenter();
    b2Vec2 slot(_slot_offset, 0);
    slot = _body->GetWorldVector(slot);
    b2Vec2 slot_position = center + slot;
    res.point = Vector(slot_position.x, slot_position.y);
    res.direction = RAD2DEG(atan2(slot.y, slot.x));
    return res;
  }


  void Car::translate_body(b2Body* body, const Real rad_angle, const b2Vec2& dest, const b2Vec2& pos) {
    const b2Vec2 body_pos = body->GetWorldCenter();
    const b2Rot rot(rad_angle);
    const b2Vec2 offset = b2Mul(rot, (body_pos - pos));
    const Real d_angle = body->GetAngle() + rad_angle;
    const b2Vec2 translate = dest - body_pos;
    const b2Vec2 new_pos = body_pos + translate + offset;
    body->SetAngularVelocity(0);
    body->SetLinearVelocity(b2Vec2(0, 0));
    body->SetTransform(new_pos, d_angle);
  }


  void Car::place(const ConnectionPoint& dest) {
    ConnectionPoint pos = position();
    const Real angle = dest.direction - pos.direction;
    const Real rad_angle = DEG2RAD(angle);
    const b2Vec2 dest_pos = vconv(dest.point);
    const b2Vec2 current_pos = vconv(pos.point);
    translate_body(_body, rad_angle, dest_pos, current_pos);
    
    BOOST_FOREACH(Wheel& wheel, _wheels) {
      translate_body(wheel.body(), rad_angle, dest_pos, current_pos);
    }
  }


  // =============================================================

  Wheel::Wheel(const Json::Value& definition) 
  {
      assert(definition.isObject());
      assert(definition.isMember("offset") && definition["offset"].isArray());
      assert(definition.isMember("width") && definition["width"].isDouble());
      assert(definition.isMember("diameter") && definition["diameter"].isDouble());
      assert(definition.isMember("mass") && definition["mass"].isDouble());
      _offset = Vector(definition["offset"][0].asDouble(), definition["offset"][1].asDouble());
      _width = definition["width"].asDouble();
      _diameter = definition["diameter"].asDouble();
      _mass = definition["mass"].asDouble();
  }

  Wheel::~Wheel() {
  }


  b2Body* Wheel::body() {
    return _body;
  }


  void Wheel::step(Real elapsed, bool accelerate, const EngineInfo& engine) {
    if(accelerate) {
      b2Vec2 force(engine.power, 0);
      force = _body->GetWorldVector(force);
      b2Vec2 point = _body->GetWorldCenter();
      _body->ApplyForce(force, point);
    }
  }
  
  void Wheel::physics_setup(b2World* world, b2Body* chassis, vector<function< void()> >& destroyers) {
    b2BodyDef body_def;
    body_def.type = b2_dynamicBody;
    body_def.linearDamping = 0.0f;
    body_def.angularDamping = CAR_ANGULAR_DAMPING;
    body_def.active = true;
    body_def.position.Set(_offset[0], _offset[1]);
    body_def.angle = 0.0f;
    _body = world->CreateBody(&body_def);
    destroyers.push_back(bind(&b2World::DestroyBody, world, _body));

    b2PolygonShape wheel_shape;
    wheel_shape.SetAsBox(_width / 2.0, _diameter / 2.0); // box2d uses half-widths here
    b2FixtureDef fixture_def;
    fixture_def.shape = &wheel_shape;
    fixture_def.friction = 0.0f;
    fixture_def.density = _mass / (_width * _diameter);
    _body->CreateFixture(&fixture_def);

    // attach our wheel to the chassis
    b2WeldJointDef jointDef;
    // limit the joint on our back-wheels
    // so they can't rotate
    jointDef.Initialize(_body, chassis, _body->GetWorldCenter());
    world->CreateJoint(&jointDef);
  }

}; // end ns::rracer











