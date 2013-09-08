#ifndef CAR_HH
#define CAR_HH
#include <json/json.h>
#include "world/world-object.hh"

namespace rracer {
  class Car : public WorldObject {

    AssetManager& _am;
    Real _length;
    Real _slot_offset;
    ConnectionPoint _position;
    string _image_name;

  public:
    Car(AssetManager&, const Json::Value&);
    virtual ~Car();

    virtual void render(OpenVGCompanion& vgc) const;
  };



};

#endif

