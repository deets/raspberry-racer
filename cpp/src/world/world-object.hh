#ifndef WORLD_OBJECT_HH
#define WORLD_OBJECT_HH

#include <string>
#include <boost/intrusive/list.hpp>
#include <boost/function.hpp>
#include <Box2D/Box2D.h>

#include "events/events.hh"
#include "gfx/openvg-companion.hh"
#include "assets/assets.hh"

using namespace std;
namespace bi = boost::intrusive;
using namespace boost;

namespace rracer {
  class World;

  class WorldObject : public bi::list_base_hook<> {

  public:
    typedef bi::list<WorldObject> WorldObjectList;

    virtual ~WorldObject() {}

    virtual void process_input_events(const InputEventVector& events, double elapsed);
    virtual void render(OpenVGCompanion& vgc) const;
    virtual void physics_setup(b2World *);

    void add_object(WorldObject *obj);

    friend class World;

  protected:

    virtual void dispatch_input_events(const InputEventVector& events, double elapsed);
    virtual void dispatch_render(OpenVGCompanion&);

    WorldObjectList _children;
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

    virtual void process_input_events(const InputEventVector& events, double elapsed);
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
    virtual void process_input_events(const InputEventVector& events, double elapsed);
  };
}; //ns::rracer

#endif
