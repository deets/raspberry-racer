#ifndef CAR_HH
#define CAR_HH
#include <vector>
#include <boost/function.hpp>
#include <json/json.h>
#include "world/world-object.hh"

#define CAR_ANGULAR_DAMPING 0.01f

using namespace boost;

namespace rracer {

  class Wheel {
    Vector _offset;
    Real _width;
    Real _diameter;
    Real _mass;
  public:
    Wheel(const Json::Value&);
    virtual ~Wheel();
    void physics_setup(b2World*, vector<function< void()> >& destroyers);
  };

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

  protected:
    vector<function< void () > > _destroyers;
    vector<Wheel> _wheels;
  public:
    Car(AssetManager&, const Json::Value&);
    virtual ~Car();

    virtual void render(OpenVGCompanion& vgc) const;
    virtual void physics_setup(b2World *);

  };



};

#endif

