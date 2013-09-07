#include "world/car.hh"

namespace rracer {

  Car::Car(AssetManager& am) 
    : _am(am)
  {
    _length = 12.0;
    _slot_offset = 3.0;
    _position.point = Vector(0, 5.5);
    _position.direction = 30.0;
  }

  Car::~Car() {
  }

  void Car::render(OpenVGCompanion& vgc) const {
    const PNGImageData& img_data = _am.image("cars/yellow.png");
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

