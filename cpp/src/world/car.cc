#include <cassert>

#include "world/car.hh"

namespace rracer {

  Car::Car(AssetManager& am, const Json::Value& car_info) 
    : _am(am)
  {
    assert(car_info.isMember("image") && car_info["image"].isString());
    assert(car_info.isMember("length") && car_info["length"].isDouble());
    assert(car_info.isMember("width") && car_info["width"].isDouble());
    assert(car_info.isMember("mass") && car_info["mass"].isDouble());
    assert(car_info.isMember("slot-offset") && car_info["slot-offset"].isDouble());
    _position.point = Vector(0, 5.5);
    _position.direction = 30.0;
    _image_name = car_info["image"].asString();
    _length = car_info["length"].asDouble();
    _width = car_info["width"].asDouble();
    _mass = car_info["mass"].asDouble();
    _slot_offset = car_info["slot-offset"].asDouble();
  }


  Car::~Car() {
    _world->DestroyBody(_body);
    _body = 0;
  }


  void Car::render(OpenVGCompanion& vgc) const {
    ConnectionPoint position;
    b2Vec2 pos = _body->GetPosition();
    position.point[0] = pos.x;
    position.point[1] = pos.y;
    position.direction = _body->GetAngle();
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
    b2PolygonShape chassis;
    chassis.SetAsBox(_length / 2.0, _width / 2.0); // box2d uses half-widths here
    b2FixtureDef fixture_def;
    fixture_def.shape = &chassis;
    fixture_def.friction = 0.0f;
    fixture_def.density = _mass / (_length * _width);
    _body->CreateFixture(&fixture_def);
  }

}; // end ns::rracer

