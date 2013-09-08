#include <cassert>

#include "world/car.hh"

namespace rracer {

  Car::Car(AssetManager& am, const Json::Value& car_info) 
    : _am(am)
  {
    assert(car_info.isMember("image") && car_info["image"].isString());
    assert(car_info.isMember("length") && car_info["length"].isDouble());
    assert(car_info.isMember("slot-offset") && car_info["slot-offset"].isDouble());
    _position.point = Vector(0, 5.5);
    _position.direction = 30.0;
    _image_name = car_info["image"].asString();
    _length = car_info["length"].asDouble();
    _slot_offset = car_info["slot-offset"].asDouble();

  }

  Car::~Car() {
  }

  void Car::render(OpenVGCompanion& vgc) const {
    const PNGImageData& img_data = _am.image(_image_name);
    // move to the middle axis of the car
    AffineTransform t = translate(Vector(-img_data.width() / 2.0, 0));
    // scale to world-size
    t = scale(_length / img_data.height()) * t;
    // 0 degrees shows to the right,
    // so adjust for the fact that
    // our cars are drawn facing front to upwards
    t = rotation(-90) * t;
    // translate so that the slot is the center
    t = translate(Vector(-_length + _slot_offset, 0)) * t;
    t = rotation(_position.direction) * t;
    t = translate(_position.point) * t;
    MatrixStacker(vgc, t);
    PaintScope(vgc, Color::black, VG_FILL_PATH | VG_STROKE_PATH);
    vgc.drawImage(img_data);
  }

  
}; // end ns::rracer

