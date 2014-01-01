#include "game/race.hh"
#include "hub/rracer-hub.hh"

namespace rracer {

  Hub* RRacerHub::s_factory() {
    return new RRacerHub();
  }


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
    if(_race) {
      delete _race;
    }
  }
  
  
  void RRacerHub::setup(OpenVGAdapter& vg, WindowAdapter& window_adapter, fs::path bundle_path) {
    _asset_manager = new AssetManager(vg, bundle_path / "resources");
    _scene_graph = new rracer::SceneGraph(window_adapter, vg);
    //  _scene_graph->fixed_frame_rate(SCENE_FRAMERATE);
    Race* race = new Race(_scene_graph, *_asset_manager, "tests/simple-test-track.json", "cars/car-one.json");
  }
  
  
  bool RRacerHub::frame(Real elapsed, const GameEventVector& events) {
    _race->step(events, elapsed);
    _scene_graph->start_frame(elapsed);
    _scene_graph->end_frame();
    return _race->has_ended();
  }

} // end ns::racer
