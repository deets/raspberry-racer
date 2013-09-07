#include "world/car.hh"

namespace rracer {

  Car::Car(AssetManager& am) 
    : _am(am)
  {

  }

  Car::~Car() {
  }

  void Car::render(OpenVGCompanion& vgc) const {
    PaintScope(vgc, Color::black, VG_FILL_PATH | VG_STROKE_PATH);
    const PNGImageData& img_data = _am.image("cars/yellow.png");
    vgc.drawImage(img_data);
  }

  
}; // end ns::rracer

