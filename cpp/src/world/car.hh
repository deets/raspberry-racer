#ifndef CAR_HH
#define CAR_HH
#include <vector>
#include <boost/function.hpp>
#include <json/json.h>
#include "world/world-object.hh"

#define CAR_ANGULAR_DAMPING 0.01f
#define DRAG_COEFFICIENT 500.0f
#define LAT_COEFFICIENT 2.5f
#define SLOT_DISTANCE 0.1f

using namespace boost;

namespace rracer {

  struct EngineInfo {
    Real power;
  };

  class SlotJoint {
  public:
    virtual Vector slot_impulse(const Vector& pivot_position, const Vector& slot_position)=0;
  };


  class FunctionSlotJoint : public SlotJoint {
  public:
    FunctionSlotJoint(Real distance_norm, Real power);
    virtual Vector slot_impulse(const Vector& pivot_position, const Vector& slot_position);
  private:
    Real _distance_norm;
    Real _power;
  };


  class PDSlotJoint : public SlotJoint {
  public:
    PDSlotJoint(Real df, Real pf);
    virtual Vector slot_impulse(const Vector& pivot_position, const Vector& slot_position);

  private:
    Real _f;
    Real _df;
    Real _pf;
    Real _last_distance;
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

  public:
    Car(AssetManager&, const Json::Value&);
    virtual ~Car();

    virtual void render(OpenVGCompanion& vgc) const;
    virtual void physics_setup(b2World *);
    virtual void process_input_events(const InputEventVector& events, double elapsed);

    ConnectionPoint position() const;
    void place(const ConnectionPoint&);
    void push_to_slot(const Vector& slot);
  private:

    AssetManager& _am;
    Real _length;
    Real _width;
    Real _pivot_offset;
    Real _mass;

    string _image_name;
    b2Body* _body;
    b2Body* _pivot_body;
    b2World* _world;
    bool _accelerate;

    void step(Real elapsed);

    static void translate_body(b2Body* body, const Real rad_angle, const b2Vec2& dest, const b2Vec2& pos);

  protected:
    vector<function< void () > > _destroyers;
    vector<Wheel> _wheels;

    EngineInfo _engine;

    shared_ptr<SlotJoint> _slot_joint;
  };



};

#endif

