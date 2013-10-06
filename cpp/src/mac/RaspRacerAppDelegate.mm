
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
#include <boost/bind.hpp>

#include "mac/common.h"
#include "gfx/openvg-companion.hh"
#include "world/race.hh"

#import "RaspRacerAppDelegate.h"

namespace fs = boost::filesystem;

@implementation RaspRacerAppDelegate

@synthesize window = _window;
@synthesize glview = _glview;

- (id) init {
  self = [super init];
  _window_adapter = 0;
  _hud = 0;
  _asset_manager = 0;
  _world = 0;
  _events = new rracer::InputEventVector();
  _world_timer = new Timer();
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

  if(_hud) {
    delete _hud;
  }

  if(_world) {
    delete _world;
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

  _asset_manager = new AssetManager(*_window_adapter, bundle_resources / "resources");
  _world = new rracer::World(*_window_adapter, *_window_adapter);
  rracer::Race* race = new rracer::Race(*_world, *_asset_manager, "tests/simple-test-track.json", "cars/car-one.json");
  _world->add_object(race);

  [_glview setRenderCallback: self];
  _timer = [NSTimer scheduledTimerWithTimeInterval: 1.0 / WORLD_FRAMERATE target: self selector: @selector(timerCallback:) userInfo: nil repeats: YES];

}

-(void) timerCallback:(NSTimer*) timer {
  [_glview setNeedsDisplay: YES];
}


-(void)render {
  if(_world) {
    double elapsed = _world_timer->elapsed();
    if(elapsed > 0.0) {
      _world->start_frame(*_events, elapsed);
      _world->end_frame();
      if(_world->has_ended()) {
	[[NSApplication sharedApplication] terminate: self];
      }
    }
  }
  _events->clear();
}


-(BOOL) convertEvent:(NSEvent*) event {
  rracer::InputEvent game_event;
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
    case 2:
      game_event.key = rracer::K_d;
      break;
    case 4:
      game_event.key = rracer::K_h;
      break;
    case 53:
      game_event.key = rracer::K_ESC;
      break;
    case 126:
      game_event.key = rracer::K_UP;
      break;
    case 125:
      game_event.key = rracer::K_DOWN;
      break;
    case 123:
      game_event.key = rracer::K_LEFT;
      break;
    case 124:
      game_event.key = rracer::K_RIGHT;
      break;
    case 49:
      game_event.key = rracer::K_SPACE;
      break;
    default:
      game_event.key = rracer::K_UNKNOWN;
      break;
    }
    if(game_event.key != rracer::K_UNKNOWN) {
      _events->push_back(game_event);
      return YES;
    }
  }
  return NO;
}
@end
