#include "game/race.hh"
#include "hub/playground-hub.hh"

namespace rracer {

  Hub* PlaygroundHub::s_factory() {
    return new PlaygroundHub();
  }


  PlaygroundHub::PlaygroundHub() 
    : _asset_manager(0)
    , _scene_graph(0)
    , _hud(0)
    , _animator(1000, 800, 1.0, 1.0)
    , _animator2(100, 10.0)
  {
  }
  
  
  PlaygroundHub::~PlaygroundHub() {
    if(_hud) {
      delete _hud;
    }
    if(_scene_graph) {
      delete _scene_graph;
    }
    if(_asset_manager) {
      delete _asset_manager;
    }
  }
  
  
  void PlaygroundHub::setup(OpenVGAdapter& vg, WindowAdapter& window_adapter, fs::path bundle_path) {
    _asset_manager = new AssetManager(vg, bundle_path / "resources");
    _scene_graph = new rracer::SceneGraph(window_adapter, vg);
    Image* image = new Image(*_asset_manager, "cars/yellow.png");
    image->animator(&_animator);
    _scene_graph->add_object(image);

    Image* image2 = new Image(*_asset_manager, "cars/yellow.png");
    image2->animator(&_animator2);
    image->add_object(image2);
  }
  
  
  bool PlaygroundHub::frame(Real elapsed, const GameEventVector& events) {
    _animator.step(elapsed);
    _animator2.step(elapsed);
    _scene_graph->start_frame(elapsed);
    _scene_graph->end_frame();
    return true;//_scene_graph->has_ended();
  }

} // end ns::racer
