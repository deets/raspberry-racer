#ifndef CAR_HH
#define CAR_HH

#include "world/world-object.hh"

namespace rracer {
  class Car : public WorldObject {

    AssetManager& _am;
    Real _length;
    Real _slot_offset;
    ConnectionPoint _position;
  public:
    Car(AssetManager&);
    virtual ~Car();

    virtual void render(OpenVGCompanion& vgc) const;
  };



};

#endif

