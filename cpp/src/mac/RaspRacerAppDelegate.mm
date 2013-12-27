
//  AppDelegate.m
//  Mac Rasbperry Racer
//
//  Created by Diez B. Roggisch on 8/18/13.
//  Copyright (c) 2013 Diez B. Roggisch. All rights reserved.
//
#include <vg/openvg.h>
#include <OpenGL/gl.h>
#include <vg/openvg.h>

#include <boost/filesystem.hpp>


#include "mac/common.h"
#include "gfx/openvg-companion.hh"
#include "hub/rracer-hub.hh"

#import "RaspRacerAppDelegate.h"

namespace fs = boost::filesystem;

@implementation RaspRacerAppDelegate

@synthesize window = _window;
@synthesize glview = _glview;

- (id) init {
  self = [super init];
  _window_adapter = 0;
  _hub = 0;
  _events = new rracer::GameEventVector();
  _world_timer = new Timer();
  return self;
}


- (void)dealloc
{
  if(_hub) {
    delete _hub;
  }

  if (_window_adapter) {
    delete _window_adapter;
  }

  if(_events) {
    delete _events;
  }

  if(_world_timer) {
    delete _world_timer;
  }

  [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
  NSLog(@"applicationDidFinishLaunching:");
  NSLog(@"glview: %@", _glview);

  NSSize size = [_glview frame].size;
  _window_adapter = new MacWindowAdapter(size.width, size.height);
  fs::path bundle_resources([[[NSBundle mainBundle] resourcePath] UTF8String]);

  _hub = new rracer::RRacerHub();
  _hub->setup(*_window_adapter, *_window_adapter, bundle_resources);

  [_glview setRenderCallback: self];
  _timer = [NSTimer scheduledTimerWithTimeInterval: 1.0 / SCENE_FRAMERATE target: self selector: @selector(timerCallback:) userInfo: nil repeats: YES];
}


-(void) timerCallback:(NSTimer*) timer {
  [_glview setNeedsDisplay: YES];
}


-(void)render {
  if(_hub) {
    double elapsed = _world_timer->elapsed();
    if(elapsed > 0.0) {
      if(_hub->frame(elapsed, *_events)) {
	[[NSApplication sharedApplication] terminate: self];
      }
    }
  }
  _events->clear();
}


-(BOOL) convertEvent:(NSEvent*) event {
  rracer::KeyEvent key_event;
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
    key_event.pressed = pressed;
    key_event.scancode = [event keyCode];

    switch(key_event.scancode) {
    case 2:
      key_event.key = rracer::K_d;
      break;
    case 4:
      key_event.key = rracer::K_h;
      break;
    case 53:
      key_event.key = rracer::K_ESC;
      break;
    case 126:
      key_event.key = rracer::K_UP;
      break;
    case 125:
      key_event.key = rracer::K_DOWN;
      break;
    case 123:
      key_event.key = rracer::K_LEFT;
      break;
    case 124:
      key_event.key = rracer::K_RIGHT;
      break;
    case 49:
      key_event.key = rracer::K_SPACE;
      break;
    default:
      key_event.key = rracer::K_UNKNOWN;
      break;
    }
    if(key_event.key != rracer::K_UNKNOWN) {
      _events->push_back(rracer::GameEvent(rracer::TimeInfo(), rracer::Event(key_event)));
      return YES;
    }
  }
  return NO;
}
@end














