#ifndef CAR_HH
#define CAR_HH
#include <vector>
#include <boost/function.hpp>
#include <json/json.h>
#include "world/world-object.hh"

#define CAR_ANGULAR_DAMPING 0.01f
#define DRAG_COEFFICIENT 500.0f
#define LAT_COEFFICIENT 2.5f

using namespace boost;

namespace rracer {

  struct EngineInfo {
    Real power;
  };

  class SlotJoint {
  public:
    virtual void physics_setup(const b2Vec2& pivot, b2World*, b2Body* chassis, vector<function< void()> >& destroyers)=0;
    virtual void push_to_slot(const ConnectionPoint& pivot_point, const ConnectionPoint& slot_point)=0;
    virtual void debug_render(DebugRenderer& debug_renderer) const=0;
  };



  class CurveJointSlotJoint : public SlotJoint, public ::RailPositionCallback {
  public:
    CurveJointSlotJoint();

    virtual void physics_setup(const b2Vec2& pivot, b2World*, b2Body* chassis, vector<function< void()> >& destroyers);
    virtual void push_to_slot(const ConnectionPoint& pivot_point, const ConnectionPoint& slot_point);

    virtual b2Vec2 GetLocalAnchorA( const b2Vec2& testPoint );
    virtual b2Vec2 GetLocalAxisA( const b2Vec2& testPoint );
    virtual float32 GetReferenceAngle( const b2Vec2& testPoint );
    virtual float32 GetTranslation( const b2Vec2& testPoint );
    virtual void debug_render(DebugRenderer& debug_renderer) const;

  private:
    b2Vec2 _local_anchor_a;
    b2Vec2 _local_axis_a;
    float32 _angle;
    float32 _translation;
    b2Body* _body;
    b2Body* _car_body;
    b2CurveJoint* _joint;

  };

  class Wheel {
    Vector _offset;
    Real _width;
    Real _diameter;
    Real _mass;

    b2Body* _body;
    b2Body* _chassis;

  public:
    Wheel(const Json::Value&);
    virtual ~Wheel();
    void physics_setup(b2World*, b2Body* chassis, vector<function< void()> >& destroyers);
    void step(Real elapsed, Real throttle, const EngineInfo&);

    b2Body* body();
  };

  class Car : public WorldObject {

  public:
    Car(AssetManager&, const Json::Value&);
    virtual ~Car();

    virtual void render(OpenVGCompanion& vgc) const;
    virtual void physics_setup(b2World *);
    virtual GameEventVector process_input_events(const GameEventVector& events, double elapsed);
    virtual void debug_render(DebugRenderer& debug_renderer) const;

    ConnectionPoint position() const;
    void place(const ConnectionPoint&);
    void push_to_slot(const ConnectionPoint& slot);
  private:

    void step(Real elapsed);
    static void translate_body(b2Body* body, const Real rad_angle, const b2Vec2& dest, const b2Vec2& pos);

    AssetManager& _am;
    Real _length;
    Real _width;
    Real _pivot_offset;
    Real _mass;

    string _image_name;
    b2Body* _body;
    b2World* _world;

    Real _throttle;

  protected:
    vector<function< void () > > _destroyers;
    vector<Wheel> _wheels;

    EngineInfo _engine;

    shared_ptr<SlotJoint> _slot_joint;
  };



};

#endif

