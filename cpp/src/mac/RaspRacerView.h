#import <Cocoa/Cocoa.h>
#include <OpenGL/gl.h>
#include <vg/openvg.h>

@interface RaspRacerView : NSOpenGLView {
  id _renderCallback;
}

@property (nonatomic, retain) id renderCallback;  

@end


