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
  _world = 0;
  _events = new InputEventVector();
  _then = clock();
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

  if(_events) {
    delete _events;
  }

  if(_world) {
    delete _world;
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

  _world = new World(*_window_adapter, *_window_adapter);

  [_glview setRenderCallback: self];
  _timer = [NSTimer scheduledTimerWithTimeInterval: 1.0 / 30.0 target: self selector: @selector(timerCallback:) userInfo: nil repeats: YES];

}

-(void) timerCallback:(NSTimer*) timer {
  [_glview setNeedsDisplay: YES];
}


-(void)render {
  clock_t now = clock();
  if(_world) {
    _world->begin(*_events, difftime(_then, now));
    _world->end();
    if(_world->has_ended()) {
      [[NSApplication sharedApplication] terminate: self];
    }
  }
  _events->clear();
  _then = now;
}


-(void) convertEvent:(NSEvent*) event {
  InputEvent game_event;
  bool is_valid = false;
  bool pressed = false;

  switch([event type]) {
  case NSKeyDown:
    is_valid = true;
    pressed = true;
    break;
  case NSKeyUp:
    is_valid = true;
    pressed = false;
    break;
  }

  if(is_valid) {
    game_event.pressed = pressed;
    game_event.scancode = [event keyCode];

    switch(game_event.scancode) {
    case 53:  // esc
      game_event.key = K_ESC;
      break;
    default:
      game_event.key = K_UNKNOWN;
      break;
    }
    _events->push_back(game_event);
  }
}
@end