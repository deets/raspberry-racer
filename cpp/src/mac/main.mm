//
//  main.m
//  Mac Rasbperry Racer
//
//  Created by Diez B. Roggisch on 8/18/13.
//  Copyright (c) 2013 Diez B. Roggisch. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "mac/RaspRacerAppDelegate.h"
#include "hub/rracer-hub.hh"

int main(int argc, char *argv[])
{
  s_set_hub_factory(&rracer::RRacerHub::s_factory);
  return NSApplicationMain(argc, (const char **)argv);
}
