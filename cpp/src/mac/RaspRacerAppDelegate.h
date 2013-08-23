#include <time.h>
#import <Cocoa/Cocoa.h>
#include "mac/mac-window-adapter.hh"
#include "assets/assets.hh"
#include "mac/RRRenderCallback.h"
#include "events/events.hh"
#include "world/world.hh"



@interface RaspRacerAppDelegate : NSObject <NSApplicationDelegate, RRRenderCallback> {
  NSOpenGLView *_glview;
  NSWindow *_window;
  NSTimer* _timer;
  
  MacWindowAdapter* _window_adapter;
  AssetManager* _asset_manager;

  InputEventVector* _events;
  World* _world;
  clock_t _then;
}
-(void) timerCallback:(NSTimer*)timer;
-(void) render;
-(void) convertEvent:(NSEvent*) event;

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet NSOpenGLView *glview;


@end
