#include "mac/RRApplication.h"


@implementation RRApplication

- (void)sendEvent:(NSEvent *)event {
  // if we didn't consume the event, pass it on
  if(![[self delegate] convertEvent: event]) {
    NSLog(@"sendEvent: %@", event);
    [super sendEvent: event];
  }
}
@end
