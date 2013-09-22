#ifndef CAR_HH
#define CAR_HH
#include <vector>
#include <boost/function.hpp>
#include <json/json.h>
#include "world/world-object.hh"

#define CAR_ANGULAR_DAMPING 0.01f

using namespace boost;

namespace rracer {

  struct EngineInfo {
    Real power;
  };

  class Wheel {
    Vector _offset;
    Real _width;
    Real _diameter;
    Real _mass;

    b2Body* _body;

  public:
    Wheel(const Json::Value&);
    virtual ~Wheel();
    void physics_setup(b2World*, b2Body* chassis, vector<function< void()> >& destroyers);
    void step(Real elapsed, bool accelerate, const EngineInfo&);

    b2Body* body();
  };

  class Car : public WorldObject {

    AssetManager& _am;
    Real _length;
    Real _width;
    Real _slot_offset;
    Real _mass;

    string _image_name;
    b2Body* _body;
    b2World* _world;
    bool _accelerate;

    void step(Real elapsed);

    static void translate_body(b2Body* body, const AffineTransform& rot, const Real rad_angle, const ConnectionPoint& dest, const ConnectionPoint& pos);

  protected:
    vector<function< void () > > _destroyers;
    vector<Wheel> _wheels;

    EngineInfo _engine;

  public:
    Car(AssetManager&, const Json::Value&);
    virtual ~Car();

    virtual void render(OpenVGCompanion& vgc) const;
    virtual void physics_setup(b2World *);
    virtual void process_input_events(const InputEventVector& events, double elapsed);

    ConnectionPoint position() const;
    void place(const ConnectionPoint&);
  };



};

#endif

