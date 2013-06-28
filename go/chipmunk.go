package chipmunk

/*
#cgo LDFLAGS /opt/chipmunk/lib/libchipmunk.a
#include <chipmunk/chipmunk.h>
*/
import "C"

func InitChipmunk()  {
	C.cpInitChipmunk();
}

type cpSpace struct {
	space *C.cpSpace;
}

func SpaceNew() *cpSpace {
	gospace := cpSpace{};
	gospace.space = C.cpSpaceNew();
	return gospace;
}