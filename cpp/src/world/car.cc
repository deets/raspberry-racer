#include <cassert>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#include "world/car.hh"

namespace rracer {


  FunctionSlotJoint::FunctionSlotJoint(Real distance_norm, Real power) 
    : _distance_norm(distance_norm)
    , _power(power)
  {

  }


  Vector FunctionSlotJoint::slot_impulse(const Vector& pivot_position, const Vector& slot_position) {
    const Vector d = slot_position - pivot_position;
    const Real distance = d.norm();
    if(distance) {
      Real f = pow(distance / _distance_norm, _power) / distance;
      return d * f;
    }
    return Vector(0, 0);
  }


  PDSlotJoint::PDSlotJoint(Real pf, Real df)
    : _df(df)
    , _pf(pf)
    , _f(0)
    , _last_distance(0)
  {
  }


  Vector PDSlotJoint::slot_impulse(const Vector& pivot_position, const Vector& slot_position) {
    const Vector d = slot_position - pivot_position;
    const Real distance = d.norm();
    if(distance) {
      Real df = 0;
      //if(distance < _last_distance) {
	df = (distance - _last_distance) * _df;
	//} else {
	
	//x}
      Real pf = distance * _pf;
      _f += pf + df;
      cerr << "pf: " << pf << " df:" << df << endl;

      _last_distance = distance;
      return d * (_f / distance);
    } else {
      _f = 0;
      _last_distance = distance;
      return Vector(0, 0);
    }
  }


  //=====================

  Car::Car(AssetManager& am, const Json::Value& car_info) 
    : _am(am)
    , _accelerate(false)
  {
    assert(car_info.isMember("image") && car_info["image"].isString());
    assert(car_info.isMember("length") && car_info["length"].isDouble());
    assert(car_info.isMember("width") && car_info["width"].isDouble());
    assert(car_info.isMember("mass") && car_info["mass"].isDouble());
    assert(car_info.isMember("pivot-offset") && car_info["pivot-offset"].isDouble());
    assert(car_info.isMember("wheels") && car_info["wheels"].isArray());
    assert(car_info.isMember("power") && car_info["power"].isDouble());
    _engine.power = car_info["power"].asDouble();
    _image_name = car_info["image"].asString();
    _length = car_info["length"].asDouble();
    _width = car_info["width"].asDouble();
    _mass = car_info["mass"].asDouble();
    _pivot_offset = car_info["pivot-offset"].asDouble();
    Json::Value json_wheels = car_info["wheels"];
    for(int i = 0; i < json_wheels.size(); ++i) {
      Json::Value json_wheel = json_wheels[i];
      _wheels.push_back(Wheel(json_wheel));
    }
    _slot_joint = shared_ptr<SlotJoint>(
	new PDSlotJoint(5.0, 5.0)
    );
    // new FunctionSlotJoint(SLOT_DISTANCE, 3));
  }


  Car::~Car() {
    BOOST_REVERSE_FOREACH(function< void ()> destroy, _destroyers) {
      destroy();
    }
    _body = 0;
  }


  void Car::render(OpenVGCompanion& vgc) const {
    const ConnectionPoint position = { 
      vconv(_body->GetWorldCenter()), 
      RAD2DEG(_body->GetAngle())
    };
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
    PaintScope p(vgc, Color::black, VG_FILL_PATH | VG_STROKE_PATH);
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


  void Car::push_to_slot(const Vector& slot) {
    b2Vec2 pos = _pivot_body->GetPosition();
    cerr << "slot: " << slot[0] << "x" << slot[1] << " pivot_body: " << pos.x << "x" << pos.y << endl;
    _pivot_body->SetTransform(vconv(slot), 0);
    _pivot_body->SetAngularVelocity(0);
    _pivot_body->SetLinearVelocity(b2Vec2(0, 0));
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

    b2BodyDef pivot_body_def;
    pivot_body_def.type = b2_kinematicBody;
    pivot_body_def.linearDamping = 0.0f;
    pivot_body_def.angularDamping = CAR_ANGULAR_DAMPING;
    pivot_body_def.active = true;
    pivot_body_def.position = vconv(position().point);
    pivot_body_def.angle = 0.0f;
    _pivot_body = world->CreateBody(&pivot_body_def);
    _destroyers.push_back(bind(&b2World::DestroyBody, _world, _pivot_body));

    // b2PolygonShape foo;
    // foo.SetAsBox(.1, .1); // box2d uses half-widths here
    // b2FixtureDef fixture_def;
    // fixture_def.shape = &foo;
    // fixture_def.friction = 0.0f;
    // fixture_def.density = 1000000.0;
    // _pivot_body->CreateFixture(&fixture_def);

    b2RopeJointDef pivot_joint_def;
    pivot_joint_def.bodyA = _body;
    pivot_joint_def.bodyB = _pivot_body;
    pivot_joint_def.localAnchorA = pivot_body_def.position;
    pivot_joint_def.localAnchorB = b2Vec2(0, 0); //pivot_body_def.position;
    pivot_joint_def.maxLength = 1.1;
    world->CreateJoint(&pivot_joint_def);
    
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
    b2Vec2 pivot(_pivot_offset, 0);
    pivot = _body->GetWorldVector(pivot);
    b2Vec2 pivot_position = center + pivot;
    res.point = Vector(pivot_position.x, pivot_position.y);
    res.direction = RAD2DEG(atan2(pivot.y, pivot.x));
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
    _pivot_body->SetTransform(current_pos, 0);
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
    const b2Vec2 center = _body->GetWorldCenter();
    b2Vec2 vel = _body->GetLinearVelocity();
    b2Vec2 rn = _body->GetWorldVector(b2Vec2(0, 1.0));
    b2Vec2 fn = _body->GetWorldVector(b2Vec2(1.0, 0));

    if(accelerate) {
      _body->ApplyForce(engine.power * fn, center);
    }
    
    _body->ApplyForce(-DRAG_COEFFICIENT * fn, center);

    Real pvel = b2Dot(rn, vel);
    b2Vec2 lat_vel = pvel * rn;
    _body->ApplyLinearImpulse(-lat_vel, center);
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











