
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
#include "world/track.hh"
#include "world/car.hh"

#import "RaspRacerAppDelegate.h"

namespace fs = boost::filesystem;

@implementation RaspRacerAppDelegate

@synthesize window = _window;
@synthesize glview = _glview;

- (id) init {
  self = [super init];
  _window_adapter = 0;
  _asset_manager = _car_asset_manager = 0;
  _world = 0;
  _events = new InputEventVector();
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

  if (_car_asset_manager) {
    delete _car_asset_manager;
  }

  if(_events) {
    delete _events;
  }

  if(_world) {
    delete _world;
  }


  if(_debug_renderer) {
    delete _debug_renderer;
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
  _car_asset_manager = new AssetManager(*_window_adapter, bundle_resources / "resources" / "cars");

  _world = new rracer::World(*_window_adapter, *_window_adapter);

  _debug_renderer = new rracer::DebugRenderer(*_window_adapter);
  _debug_renderer->SetFlags(b2Draw::e_shapeBit);
  _world->set_debug_renderer(_debug_renderer);
  // we want all drawing at (0,0) centered around the middle of the screen

  rracer::Rect screen_rect(0, 0, size.width, size.height);
  rracer::Track* track = new rracer::Track(*_asset_manager, "tests/simple-test-track.json");

  rracer::AffineTransformator* t = new rracer::AffineTransformator(screen_rect.fit(track->bounds() * 1.1));

  rracer::Car* car = new rracer::Car(*_car_asset_manager, _car_asset_manager->json("car-one.json"));
  car->physics_setup(_world->world());
  t->add_object(track);
  t->add_object(car);
  _debug_renderer->world_transform(boost::bind(&rracer::AffineTransformator::affine_transform, t));
  _world->add_object(t);

  [_glview setRenderCallback: self];
  _timer = [NSTimer scheduledTimerWithTimeInterval: 1.0 / 30.0 target: self selector: @selector(timerCallback:) userInfo: nil repeats: YES];

}

-(void) timerCallback:(NSTimer*) timer {
  [_glview setNeedsDisplay: YES];
}


-(void)render {
  if(_world) {
    double elapsed = _world_timer->elapsed();
    if(elapsed > 0.0) {
      _world->begin(*_events, elapsed);
      _world->end();
      if(_world->has_ended()) {
	[[NSApplication sharedApplication] terminate: self];
      }
    }
  }
  _events->clear();
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
