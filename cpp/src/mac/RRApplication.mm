#include "mac/RRApplication.h"


@implementation RRApplication

- (void)sendEvent:(NSEvent *)event {
  NSLog(@"sendEvent: %@", event);
  [[self delegate] convertEvent: event];
  [super sendEvent: event];
}
@end
