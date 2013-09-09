#include <time.h>
#import <Cocoa/Cocoa.h>
#include "mac/mac-window-adapter.hh"
#include "assets/assets.hh"
#include "mac/RRRenderCallback.h"
#include "events/events.hh"
#include "world/world.hh"
#include "common/time.hh"
#include "RaspRacerView.h"


@interface RaspRacerAppDelegate : NSObject <NSApplicationDelegate, RRRenderCallback> {
  RaspRacerView *_glview;
  NSWindow *_window;
  NSTimer* _timer;

  MacWindowAdapter* _window_adapter;
  AssetManager* _asset_manager;
  AssetManager* _car_asset_manager;

  InputEventVector* _events;
  rracer::World* _world;
  Timer* _world_timer;
}
-(void) timerCallback:(NSTimer*)timer;
-(void) render;
-(void) convertEvent:(NSEvent*) event;

@property (assign) IBOutlet NSWindow* window;
@property (assign) IBOutlet RaspRacerView* glview;


@end
