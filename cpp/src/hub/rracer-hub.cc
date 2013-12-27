#include "scene/race.hh"
#include "hub/rracer-hub.hh"

namespace rracer {

  RRacerHub::RRacerHub() 
    : _asset_manager(0)
    , _scene_graph(0)
    , _hud(0)
  {
  }
  
  
  RRacerHub::~RRacerHub() {
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
  
  
  void RRacerHub::setup(OpenVGAdapter& vg, WindowAdapter& window_adapter, fs::path bundle_path) {
    _asset_manager = new AssetManager(vg, bundle_path / "resources");
    _scene_graph = new rracer::SceneGraph(window_adapter, vg);
    //  _scene_graph->fixed_frame_rate(SCENE_FRAMERATE);
    Race* race = new Race(*_scene_graph, *_asset_manager, "tests/simple-test-track.json", "cars/car-one.json");
    _scene_graph->add_object(race);
  }
  
  
  bool RRacerHub::frame(Real elapsed, const GameEventVector& events) {
    _scene_graph->start_frame(events, elapsed);
    _scene_graph->end_frame();
    return _scene_graph->has_ended();
  }

} // end ns::racer
