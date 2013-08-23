#import <Cocoa/Cocoa.h>
#include "mac/mac-window-adapter.hh"
#include "assets/assets.hh"
#include "mac/RRRenderCallback.h"

@interface RaspRacerAppDelegate : NSObject <NSApplicationDelegate, RRRenderCallback> {
  NSOpenGLView *_glview;
  NSWindow *_window;
  NSTimer* _timer;
  
  MacWindowAdapter* _window_adapter;
  AssetManager* _asset_manager;

}
-(void) timerCallback:(NSTimer*)timer;
-(void) render;

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet NSOpenGLView *glview;


@end
