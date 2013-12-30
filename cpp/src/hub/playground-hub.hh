#ifndef RRACER_HUB
#define RRACER_HUB

#include "debug/hud.hh"
#include "hub/hub.hh"

namespace rracer {

  class CircleAnimator : public Animator {
  public:
    CircleAnimator(Real radius, Real speed)
      : _radius(radius)
      , _speed(speed)
      , _phase(0)
    {
    }

    void step(Real elapsed) {
      _phase += elapsed;
    }

    Real scale() {
      return .5;
    }

    ConnectionPoint position() {
      const Real h = _phase * _speed;
      ConnectionPoint res;
      res.position = Vector(cos(h) * _radius, sin(h) * _radius);
      res.direction = 0;
      return res;
    }


  private:
    Real _radius;
    Real _speed;
    Real _phase;
  };


  class PlaygroundHub : public Hub {

  public:
    PlaygroundHub();
    virtual ~PlaygroundHub();

    virtual void setup(OpenVGAdapter& vg, WindowAdapter& window_adapter, fs::path bundle_path);
    virtual bool frame(Real elapsed, const GameEventVector& events);

    static Hub* s_factory();

  private:

    AssetManager* _asset_manager;
    HUD* _hud;
    SceneGraph* _scene_graph;

    LissajouAnimator _animator;
    CircleAnimator _animator2;
  };

}; // end ns::rracer

#endif
