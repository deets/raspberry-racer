#import "RaspRacerView.h"


@implementation RaspRacerView

#define GLErrorCheck { int err; if( err = glGetError()) { NSLog(@"glGetError(): %d", err); } }

- (void) prepareOpenGL {
  NSLog(@"prepareOpenGL");
  NSRect frame = [self frame];
  VGint width, height;
  width = (VGint)frame.size.width;
  height = (VGint)frame.size.height;
  NSLog(@"width: %i height: %i", width, height);
  vgCreateContextSH(width, height);

  _paint = vgCreatePaint();
  vgSetPaint(_paint, VG_FILL_PATH );
  VGfloat color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
  vgSetParameterfv(_paint, VG_PAINT_COLOR, 4, &color[0]);

  NSLog(@"end: prepareOpenGL");
  
}


- (void)drawRect:(NSRect)dirtyRect {
  //  NSLog(@"drawRect:");
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);

  NSRect frame = [self frame];
  vgResizeSurfaceSH((VGint)frame.size.width, (VGint)frame.size.height);

  vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
  GLErrorCheck;
  vgLoadIdentity();
  GLErrorCheck;

  //  vgTranslate(frame.size.width / 2.0, frame.size.height / 2.0 );
  GLErrorCheck;

  
  VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
  GLErrorCheck;

  VGfloat pathData[10];
  VGubyte pathSegs[6];

  pathSegs[0] = VG_MOVE_TO_ABS;
  pathData[0] = 40.0f; pathData[1] = 40.0f;
  pathSegs[1] = VG_LINE_TO_ABS;
  pathData[2] = 236.0f; pathData[3] = 170.0f;
  pathSegs[2] = VG_LINE_TO_ABS;
  pathData[4] = 20.0f; pathData[5] = 170.0f;
  pathSegs[3] = VG_LINE_TO_ABS;
  pathData[6] = 216.0f; pathData[7] = 40.0f;
  pathSegs[4] = VG_LINE_TO_ABS;
  pathData[8] = 128.0f; pathData[9] = 236.0f;
  pathSegs[5] = VG_CLOSE_PATH;

  vgAppendPathData(path, 6, pathSegs, pathData);
  GLErrorCheck;

  vguEllipse(path, 0, 0, 100.0, 100.0);
  // GLErrorCheck;
  vgSetPaint( _paint, VG_FILL_PATH );

  vgDrawPath(path, VG_FILL_PATH | VG_STROKE_PATH);
  GLErrorCheck;
  vgDestroyPath(path);
  GLErrorCheck;
  glFlush();
  GLErrorCheck;
}


@end
