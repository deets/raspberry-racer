//
//  AppDelegate.m
//  Mac Rasbperry Racer
//
//  Created by Diez B. Roggisch on 8/18/13.
//  Copyright (c) 2013 Diez B. Roggisch. All rights reserved.
//

#import "RaspRacerAppDelegate.h"

@implementation RaspRacerAppDelegate

@synthesize window = _window;
@synthesize glview = _glview;

- (void)dealloc
{
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
  NSLog(@"applicationDidFinishLaunching:");
  NSLog(@"glview: %@", _glview);

  _timer = [NSTimer scheduledTimerWithTimeInterval: 1.0 / 30.0 target: self selector: @selector(timerCallback:) userInfo: nil repeats: YES];

}

-(void) timerCallback:(NSTimer*) timer {
  [_glview setNeedsDisplay: YES];
}



@end
