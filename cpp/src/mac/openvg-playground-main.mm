//
//  main.m
//  Mac Rasbperry Racer
//
//  Created by Diez B. Roggisch on 8/18/13.
//  Copyright (c) 2013 Diez B. Roggisch. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "mac/RaspRacerAppDelegate.h"
#include "hub/playground-hub.hh"

int main(int argc, char *argv[])
{
  s_set_hub_factory(&rracer::PlaygroundHub::s_factory);
  return NSApplicationMain(argc, (const char **)argv);
}
