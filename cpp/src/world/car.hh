#ifndef CAR_HH
#define CAR_HH

#include "world/world-object.hh"

namespace rracer {
  class Car : public WorldObject {

    AssetManager& _am;

  public:
    Car(AssetManager&);
    virtual ~Car();

    virtual void render(const OpenVGCompanion& vgc) const;
  };



};

#endif

