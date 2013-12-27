#include "scene/race.hh"
#include "hub/playground-hub.hh"

namespace rracer {

  Hub* PlaygroundHub::s_factory() {
    return new PlaygroundHub();
  }


  PlaygroundHub::PlaygroundHub() 
    : _asset_manager(0)
    , _scene_graph(0)
    , _hud(0)
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
    //  _scene_graph->fixed_frame_rate(SCENE_FRAMERATE);
  }
  
  
  bool PlaygroundHub::frame(Real elapsed, const GameEventVector& events) {
    _scene_graph->start_frame(events, elapsed);
    _scene_graph->end_frame();
    return _scene_graph->has_ended();
  }

} // end ns::racer
