#ifndef WORLD_OBJECT_HH
#define WORLD_OBJECT_HH

#include <string>
#include <boost/intrusive/list.hpp>
#include <boost/function.hpp>
#include <Box2D/Box2D.h>

#include "events/events.hh"
#include "gfx/openvg-companion.hh"
#include "assets/assets.hh"
#include "debug/debug.hh"


using namespace std;
namespace bi = boost::intrusive;
using namespace boost;

namespace rracer {
  class World;

  class WorldObject : public bi::list_base_hook<> {

  public:
    typedef bi::list<WorldObject> WorldObjectList;
    typedef WorldObjectList::iterator wo_iterator;

    WorldObject();

    virtual ~WorldObject() {}

    virtual void process_input_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event);
    virtual void render(OpenVGCompanion& vgc) const;
    virtual void physics_setup(b2World *);
    virtual void debug_render(DebugRenderer& debug_renderer) const;

    void add_object(WorldObject *obj);

    string name() const;
    void name(const string n);

    wo_iterator begin();
    wo_iterator end();

    friend class World;

  protected:

    virtual void dispatch_input_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event);
    virtual void dispatch_render(OpenVGCompanion&);

    WorldObjectList _children;

  private:
    string _name;
  };



  class Image : public WorldObject {

    AssetManager& _am;
    string _asset_name;

  public:
    Image(AssetManager& am, string asset_name);
    virtual void render(OpenVGCompanion& vgc) const;

  };


  class Translator : public WorldObject {

    float _x, _y;
  public:
    Translator(float x, float y);
    virtual void render(OpenVGCompanion& vgc) const;
  };


  class LissajouAnimator : public WorldObject {

    float _width, _height, _alpha, _beta, _phase;
  public:
    LissajouAnimator(float width, float height, float alpha, float _beta);

    virtual void process_input_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event);
    virtual void render(OpenVGCompanion& vgc) const;

  };


  class AffineTransformator : public WorldObject {
    AffineTransform _t;
  public:
    AffineTransformator(const AffineTransform& t);

    AffineTransform affine_transform() const;
    virtual void dispatch_render(OpenVGCompanion&);
  };


  class CircleRenderer : public WorldObject {
    function< Vector ()> _position_callback;
    Real _radius;
    const Color& _color;
  public:
    CircleRenderer(function< Vector () >, Real radius, const Color&);
    virtual void render(OpenVGCompanion& vgc) const;

  };


  class KeyAction : public WorldObject {
    function< void () > _key_callback;
    EKeys _key;

  public:
    KeyAction(EKeys key, function< void () >);
    virtual void process_input_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event);
  };
}; //ns::rracer

#endif
