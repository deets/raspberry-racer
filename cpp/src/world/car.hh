#ifndef CAR_HH
#define CAR_HH
#include <json/json.h>
#include "world/world-object.hh"

#define CAR_ANGULAR_DAMPING 0.01f

namespace rracer {
  class Car : public WorldObject {

    AssetManager& _am;
    Real _length;
    Real _width;
    Real _slot_offset;
    Real _mass;

    ConnectionPoint _position;
    string _image_name;
    b2Body* _body;
    b2World* _world;

  public:
    Car(AssetManager&, const Json::Value&);
    virtual ~Car();

    virtual void render(OpenVGCompanion& vgc) const;

    virtual void physics_setup(b2World *);

  };



};

#endif

