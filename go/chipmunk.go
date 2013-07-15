package chipmunk

/*
 #cgo LDFLAGS: /opt/chipmunk/lib/libchipmunk.a -lm
 #include <chipmunk/chipmunk.h>
 */
import "C"

func InitChipmunk()  {
	C.cpInitChipmunk();
}

type CpSpace struct {
	space *C.cpSpace;
}

func SpaceNew() *CpSpace {
	gospace := CpSpace{};
	gospace.space = C.cpSpaceNew();
	return &gospace;
}