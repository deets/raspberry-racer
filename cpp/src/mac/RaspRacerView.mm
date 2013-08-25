#include "mac/common.h"

#import "RaspRacerView.h"



@implementation RaspRacerView

@synthesize renderCallback = _renderCallback;

- (void) prepareOpenGL {
  NSLog(@"prepareOpenGL");
  NSRect frame = [self frame];
  VGint width, height;
  width = (VGint)frame.size.width;
  height = (VGint)frame.size.height;
  NSLog(@"width: %i height: %i", width, height);
  vgCreateContextSH(width, height);

  NSLog(@"end: prepareOpenGL");

}


- (void)drawRect:(NSRect)dirtyRect {
  //  NSLog(@"drawRect:");
  NSRect frame = [self frame];
  vgResizeSurfaceSH((VGint)frame.size.width, (VGint)frame.size.height);
  GLErrorCheck;

  [_renderCallback render];

  GLErrorCheck;
  glFlush();
  GLErrorCheck;
}


@end
