//
//  AppDelegate.m
//  Mac Rasbperry Racer
//
//  Created by Diez B. Roggisch on 8/18/13.
//  Copyright (c) 2013 Diez B. Roggisch. All rights reserved.
//
#include <vg/openvg.h>
#include <OpenGL/gl.h>
#include <vg/openvg.h>

#include "mac/common.h"
#include "gfx/openvg-companion.hh"

#import "RaspRacerAppDelegate.h"


@implementation RaspRacerAppDelegate

@synthesize window = _window;
@synthesize glview = _glview;

- (id) init {
  self = [super init];
  _window_adapter = 0;
  _asset_manager = 0;
  return self;
}


- (void)dealloc
{
  if (_window_adapter) {
    delete _window_adapter;
  }
  if (_asset_manager) {
    delete _asset_manager;
  }
  [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
  NSLog(@"applicationDidFinishLaunching:");
  NSLog(@"glview: %@", _glview);

  NSSize size = [_glview frame].size;
  
  _window_adapter = new MacWindowAdapter(size.width, size.height);

  _asset_manager = new AssetManager(_window_adapter);

  [_glview setRenderCallback: self];
  _timer = [NSTimer scheduledTimerWithTimeInterval: 1.0 / 30.0 target: self selector: @selector(timerCallback:) userInfo: nil repeats: YES];

  

}

-(void) timerCallback:(NSTimer*) timer {
  [_glview setNeedsDisplay: YES];
}


-(void)render {

  
  VGPaint paint;

  paint = vgCreatePaint();
  vgSetPaint(paint, VG_FILL_PATH );
  VGfloat color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
  vgSetParameterfv(paint, VG_PAINT_COLOR, 4, &color[0]);
  
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
  vgSetPaint( paint, VG_FILL_PATH );

  vgDrawPath(path, VG_FILL_PATH | VG_STROKE_PATH);
  GLErrorCheck;
  vgDestroyPath(path);
  GLErrorCheck;

  vgDestroyPaint(paint);

  OpenVGCompanion vgc(*_window_adapter);

  VGfloat yellow[4] = {1.0, 1.0, .0, 1.0};
  vgc.setFillColor(yellow);
  vgc.drawText(_asset_manager->get_font(), 20, 20, "Hallo!", 30);  

}
@end
