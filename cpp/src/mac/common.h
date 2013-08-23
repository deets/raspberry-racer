#ifndef COMMON_H
#define COMMON_H

#define GLErrorCheck { int err; if( err = glGetError()) { NSLog(@"glGetError(): %d", err); } }

#endif
