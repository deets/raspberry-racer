#include <time.h>
#import <Cocoa/Cocoa.h>
#include "mac/mac-window-adapter.hh"
#include "mac/RRRenderCallback.h"
#include "events/events.hh"
#include "common/time.hh"
#include "RaspRacerView.h"
#include "hub/hub.hh"


@interface RaspRacerAppDelegate : NSObject <NSApplicationDelegate, RRRenderCallback> {
  RaspRacerView *_glview;
  NSWindow *_window;
  NSTimer* _timer;

  MacWindowAdapter* _window_adapter;
  Timer* _world_timer;
  rracer::Hub* _hub;
  rracer::GameEventVector *_events;
}

-(void) timerCallback:(NSTimer*)timer;
-(void) render;
-(BOOL) convertEvent:(NSEvent*) event;

@property (assign) IBOutlet NSWindow* window;
@property (assign) IBOutlet RaspRacerView* glview;


@end
