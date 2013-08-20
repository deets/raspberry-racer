#import <Cocoa/Cocoa.h>

@interface RaspRacerAppDelegate : NSObject <NSApplicationDelegate> {
  NSOpenGLView *_glview;
  NSWindow *_window;
  NSTimer* _timer;
}
-(void) timerCallback:(NSTimer*)timer;

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet NSOpenGLView *glview;


@end
