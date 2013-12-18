#include <cassert>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>


#include "world/car.hh"

namespace rracer {

  CurveJointSlotJoint::CurveJointSlotJoint() 
    : _body(0)
    , _joint(0)
  {
  }


  void CurveJointSlotJoint::debug_render(DebugRenderer& debug_renderer) const {
    debug_renderer.render_arrow(_local_anchor_a, _local_axis_a, Color::red);
  }

 
  void CurveJointSlotJoint::physics_setup(const b2Vec2& pivot, b2World* world, b2Body* car_body, vector<function< void()> >& destroyers) {
    _car_body = car_body;
    b2BodyDef kd;
    kd.type = b2_staticBody;
    kd.position.Set(0,0);
    _body = world->CreateBody(&kd);
    destroyers.push_back(bind(&b2World::DestroyBody, world, _body));

    {
      b2CurveJointDef jointDef;
      jointDef.bodyA = _body;
      jointDef.bodyB = car_body;
      jointDef.localAnchorB = pivot;
      
      jointDef.enableMotor = false;
      jointDef.motorSpeed = 10;
      jointDef.maxMotorForce = 1000;
      
      jointDef.enableLimit = true;
      jointDef.lowerTranslation = -20;
      jointDef.upperTranslation = 20;
      _joint = (b2CurveJoint*)world->CreateJoint(&jointDef);
      _joint->SetRailPositionCallback(this);
      _joint->SetPreventRotation(false);
    }

  }


  void CurveJointSlotJoint::push_to_slot(const ConnectionPoint& pivot_point, const ConnectionPoint& slot_point) {
    _local_anchor_a = vconv(slot_point.position);
    _local_axis_a = vconv(slot_point.tangent());
    _angle = DEG2RAD(slot_point.direction);
    _translation = (pivot_point.position - slot_point.position).norm();
  }



  b2Vec2 CurveJointSlotJoint::GetLocalAnchorA( const b2Vec2& testPoint ) {
    return _local_anchor_a;
  }


  b2Vec2 CurveJointSlotJoint::GetLocalAxisA( const b2Vec2& testPoint ) {
    return _local_axis_a;
  }


  float32 CurveJointSlotJoint::GetReferenceAngle( const b2Vec2& testPoint ) {
    return _angle;
    //return _car_body->GetAngle();
  }


  float32 CurveJointSlotJoint::GetTranslation( const b2Vec2& testPoint ) {
    return _translation;
  }


  //=====================

  Car::Car(AssetManager& am, const Json::Value& car_info) 
    : _am(am)
    , _throttle(0.0)
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
    _slot_joint = shared_ptr<SlotJoint>(new CurveJointSlotJoint());
  }


  Car::~Car() {
    BOOST_REVERSE_FOREACH(function< void ()> destroy, _destroyers) {
      destroy();
    }
    _body = 0;
  }

  void Car::debug_render(DebugRenderer& debug_renderer) const {
    if(_slot_joint) {
      _slot_joint->debug_render(debug_renderer);
    }
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
    t = translate(position.position) * t;
    MatrixStacker(vgc, t);
    PaintScope p(vgc, Color::black, VG_FILL_PATH | VG_STROKE_PATH);
    vgc.drawImage(img_data);
  }


  void Car::step(Real elapsed) {
    BOOST_FOREACH(Wheel& wheel, _wheels) {
      wheel.step(elapsed, _throttle, _engine);
    }
  }


  GameEventVector Car::process_input_events(const GameEventVector& events, double elapsed) {
    BOOST_FOREACH(const GameEvent event, events) {
      KeyEvent key_event = boost::get<KeyEvent>(event);
      switch(key_event.key) {
      case K_UP:
	_throttle = key_event.pressed ? 1.0 : 0.0;
	break;
      case K_DOWN:
	_throttle = key_event.pressed ? -1.0 : 0.0;
	break;
      }
    }
    this->step(elapsed);
    GameEventVector next_frame_events;
    return next_frame_events;
  }


  void Car::push_to_slot(const ConnectionPoint& slot) {
    _slot_joint->push_to_slot(this->position(), slot);
    // _pivot_body->SetTransform(vconv(slot.position), 0);
    // _pivot_body->SetAngularVelocity(0);
    // _pivot_body->SetLinearVelocity(b2Vec2(0, 0));
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
    _slot_joint->physics_setup(b2Vec2(_pivot_offset, 0), world, _body, _destroyers);
  }


  ConnectionPoint Car::position() const {
    ConnectionPoint res;
    b2Vec2 center = _body->GetWorldCenter();
    b2Vec2 pivot(_pivot_offset, 0);
    pivot = _body->GetWorldVector(pivot);
    b2Vec2 pivot_position = center + pivot;
    res.position = vconv(pivot_position);
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
    const b2Vec2 dest_pos = vconv(dest.position);
    const b2Vec2 current_pos = vconv(pos.position);
    //_pivot_body->SetTransform(current_pos, 0);
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


  void Wheel::step(Real elapsed, Real throttle, const EngineInfo& engine) {
    const b2Vec2 center = _body->GetWorldCenter();
    const b2Vec2 car_velocity = _chassis->GetLinearVelocity();
    const b2Vec2 right_normal = _body->GetWorldVector(b2Vec2(0, 1.0));
    const b2Vec2 front_normal = _body->GetWorldVector(b2Vec2(1.0, 0));

    b2Vec2 total_force(0, 0);

    if(throttle != 0.0) {
      total_force += throttle * engine.power * front_normal;
    }

    Real front_velocity = b2Dot(front_normal, car_velocity);
    total_force += -DRAG_COEFFICIENT * front_velocity * front_normal;

    _body->ApplyForce(total_force, center, true);

    // this kills the drift
    b2Vec2 lateral_velocity = b2Dot(right_normal, car_velocity) * right_normal;
    _body->ApplyLinearImpulse(_chassis->GetMass() * -lateral_velocity, center, true);
  }

  
  void Wheel::physics_setup(b2World* world, b2Body* chassis, vector<function< void()> >& destroyers) {
    _chassis = chassis;
    {
      b2BodyDef body_def;
      body_def.type = b2_dynamicBody;
      body_def.linearDamping = 0.0f;
      body_def.angularDamping = CAR_ANGULAR_DAMPING;
      body_def.active = true;
      body_def.position.Set(_offset[0], _offset[1]);
      body_def.angle = 0.0f;
      _body = world->CreateBody(&body_def);
      destroyers.push_back(bind(&b2World::DestroyBody, world, _body));
    }
    
    {
      // attach our wheel to the chassis
      b2PolygonShape wheel_shape;
      wheel_shape.SetAsBox(_width / 2.0, _diameter / 2.0); // box2d uses half-widths here
      b2FixtureDef fixture_def;
      fixture_def.shape = &wheel_shape;
      fixture_def.friction = 0.0f;
      fixture_def.density = _mass / (_width * _diameter);
      _body->CreateFixture(&fixture_def);
      
      // limit the joint on our back-wheels
      // so they can't rotate
      b2WeldJointDef jointDef;
      jointDef.Initialize(_body, chassis, _body->GetWorldCenter());
      world->CreateJoint(&jointDef);
    }
  }

}; // end ns::rracer











