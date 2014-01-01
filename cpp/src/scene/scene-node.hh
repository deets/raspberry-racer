#ifndef SCENE_NODE_HH
#define SCENE_NODE_HH

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

  class Animator {
  public:
    virtual ~Animator();

    virtual ConnectionPoint position();
    virtual Real scale();

  };

  class SceneGraph;

  class SceneNode : public bi::list_base_hook<> {

  public:
    typedef bi::list<SceneNode> SceneNodeList;
    typedef SceneNodeList::iterator iterator;

    SceneNode();

    virtual ~SceneNode() {}

    virtual void render(OpenVGCompanion& vgc) const;
    virtual void debug_render(DebugRenderer& debug_renderer) const;

    SceneNode* parent() const;
    void parent(SceneNode* parent);
    void add_object(SceneNode *obj);

    string name() const;
    void name(const string& n);

    iterator begin();
    iterator end();

    void animator(Animator* animator);
    Animator* animator() const;

    static AffineTransform animator_transform(Animator* animator);

    friend class SceneGraph;

  protected:

    virtual void dispatch_render(OpenVGCompanion&);

    // virtual void dispatch_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event);
    // void object_added(SceneNode* parent, SceneNode* child);
    // virtual void on_object_added(SceneNode* parent, SceneNode* child);

    SceneNodeList _children;

  private:
    string _name;
    SceneNode* _parent;
    Animator* _animator;
  };



  class Image : public SceneNode {

    AssetManager& _am;
    string _asset_name;

  public:
    Image(AssetManager& am, string asset_name);
    virtual void render(OpenVGCompanion& vgc) const;

  };


  class Translator : public SceneNode {

    float _x, _y;
  public:
    Translator(float x, float y);
    virtual void render(OpenVGCompanion& vgc) const;
  };


  class LissajouAnimator : public Animator {

    float _width, _height, _alpha, _beta, _phase;
  public:
    LissajouAnimator(float width, float height, float alpha, float _beta);

    void step(Real elapsed);
    virtual ConnectionPoint position();
    virtual Real scale();

  };


  class AffineTransformator : public SceneNode {
    AffineTransform _t;
  public:
    AffineTransformator(const AffineTransform& t);

    AffineTransform affine_transform() const;
    virtual void dispatch_render(OpenVGCompanion&);
  };


  class CircleRenderer : public SceneNode {
    function< Vector ()> _position_callback;
    Real _radius;
    const Color& _color;
  public:
    CircleRenderer(function< Vector () >, Real radius, const Color&);
    virtual void render(OpenVGCompanion& vgc) const;

  };


  class KeyAction : public SceneNode {
    function< void () > _key_callback;
    EKeys _key;

  public:
    KeyAction(EKeys key, function< void () >);
    virtual void process_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event);
  };
}; //ns::rracer

#endif
