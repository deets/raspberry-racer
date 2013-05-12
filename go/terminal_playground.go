package main

import (
	"fmt";
	"syscall";
	"os";	
	"log";
	"unsafe";
	"time";
	//"io/ioutil";
	"github.com/ajstarks/openvg";
)


// Control Characters
const (
	vEOF     = iota // ICANON
	vEOL            // ICANON
	vEOL2           // ICANON together with IEXTEN
	vERASE          // ICANON
	vWERASE         // ICANON together with IEXTEN
	vKILL           // ICANON
	vREPRINT        // ICANON together with IEXTEN
	spareCC1        // Spare 1
	vINTR           // ISIG
	vQUIT           // ISIG
	vSUSP           // ISIG
	vDSUSP          // ISIG together with IEXTEN
	vSTART          // IXON, IXOFF
	vSTOP           // IXON, IXOFF
	vLNEXT          // IEXTEN
	vDISCARD        // IEXTEN
	vMIN            // !ICANON
	vTIME           // !ICANON
	vSTATUS         // ICANON together with IEXTEN
	spareCC2        // Spare 2
	nCC             // Number of control chars
)

type tcflag_t uint;
type cc_t uint8;
type speed_t uint;

type termios struct {
	c_iflag, c_oflag, c_cflag, c_lflag tcflag_t;
	c_line cc_t;
	c_cc [nCC]cc_t;
	c_ispeed, c_ospeed speed_t;
}

type kbd_repeat struct {
	delay int;
	period int;
}

const (
	ECHOKE     tcflag_t = 0x00000001 // visual erase for line kill
	ECHOE      tcflag_t = 0x00000002 // visually erase chars
	ECHOK      tcflag_t = 0x00000004 // echo NL after line kill
	ECHO       tcflag_t = 0x00000008 // enable echoing
	ECHONL     tcflag_t = 0x00000010 // echo NL even if ECHO is off
	ECHOPRT    tcflag_t = 0x00000020 // visual erase mode for hardcopy
	ECHOCTL    tcflag_t = 0x00000040 // echo control chars as ^(Char)
	ISIG       tcflag_t = 0x00000080 // enable signals INTR, QUIT, [D]SUSP
	ICANON     tcflag_t = 0x00000100 // canonicalize input lines
	ALTWERASE  tcflag_t = 0x00000200 // use alternate WERASE algorithm
	IEXTEN     tcflag_t = 0x00000400 // enable DISCARD and LNEXT
	EXTPROC    tcflag_t = 0x00000800 // external processing
	TOSTOP     tcflag_t = 0x00400000 // stop background jobs from output
	FLUSHO     tcflag_t = 0x00800000 // output being flushed (state)
	NOKERNINFO tcflag_t = 0x02000000 // no kernel output from VSTATUS
	PENDIN     tcflag_t = 0x20000000 // XXX retype pending input (state)
	NOFLSH     tcflag_t = 0x80000000 // don't flush after interrupt
)

// Input Flags
const (
	IGNBRK  tcflag_t = 0x00000001 // ignore BREAK condition
	BRKINT  tcflag_t = 0x00000002 // map BREAK to SIGINTR
	IGNPAR  tcflag_t = 0x00000004 // ignore (discard) parity errors
	PARMRK  tcflag_t = 0x00000008 // mark parity and framing errors
	INPCK   tcflag_t = 0x00000010 // enable checking of parity errors
	ISTRIP  tcflag_t = 0x00000020 // strip 8th bit off chars
	INLCR   tcflag_t = 0x00000040 // map NL into CR
	IGNCR   tcflag_t = 0x00000080 // ignore CR
	ICRNL   tcflag_t = 0x00000100 // map CR to NL (ala CRMOD)
	IXON    tcflag_t = 0x00000200 // enable output flow control
	IXOFF   tcflag_t = 0x00000400 // enable input flow control
	IXANY   tcflag_t = 0x00000800 // any char will restart after stop
	IMAXBEL tcflag_t = 0x00002000 // ring bell on input queue full
	IUTF8   tcflag_t = 0x00004000 // maintain state for UTF-8 VERASE
)


func test(fd uintptr, use_scancodes bool) (func()) {
	var original_settings termios;
	
	resetters := []func(){};

	reset := func() {
		log.Printf("resetting terminal");
		for i := 0; i < len(resetters); i++ {
			resetters[i]();
		}
	}

	// get old terminal settings and store them
	// for reset
	_, _, errno := syscall.RawSyscall(
		syscall.SYS_IOCTL,
		fd,
		uintptr(syscall.TCGETS),
		uintptr(unsafe.Pointer(&original_settings)));
	if errno != 0 {
		panic("reading termios settings failed, that can't happen!");
	}

	var raw_settings termios;
	raw_settings = original_settings;
	// turn off terminal buffering
	raw_settings.c_lflag = ^(ECHO | ICANON | IEXTEN | ISIG);

	_, _, errno = syscall.RawSyscall(
		syscall.SYS_IOCTL,
		fd,
		uintptr(syscall.TCSETS),
		uintptr(unsafe.Pointer(&raw_settings)));

	if errno != 0 {
		panic("setting termios settings failed, that can't happen!");
	}

	resetters = append(resetters, func() {
		syscall.RawSyscall(
			syscall.SYS_IOCTL,
			fd,
			uintptr(syscall.TCSETS),
			uintptr(unsafe.Pointer(&original_settings)));
	});

	var kbdrep_s kbd_repeat;
	kbdrep_s.delay = 100000;
	kbdrep_s.period = 100000;

    if use_scancodes {
        // make linux emit scan-codes
        _, _, errno = syscall.RawSyscall(
		   syscall.SYS_IOCTL,
		  0,
		  uintptr(0x4B45), // KDSKBMODE
		  uintptr(0)); // K_RAW

	   if errno != 0 {
			reset();
			panic("setting raw kb mode failed, that can't happen!");
	    }

		resetters = append(resetters, func() {
			syscall.RawSyscall(
				syscall.SYS_IOCTL,
				0,
				uintptr(0x4B45), // KDSKBMODE
				uintptr(1)); // K_XLATE
		});
		// turn off keyboard-repeat
		// the currently used values
		// should be stored into the
		// passed structure, so we can just reset 
		// with it later. But that doesn't 
		// happen, so we below use 
		// values to reset gleaned form
		// my PI's terminal
		
		_, _, errno = syscall.RawSyscall(
			syscall.SYS_IOCTL,
			0,
			uintptr(0x4B52), // KDKBDREP  
			uintptr(unsafe.Pointer(&kbdrep_s)));
		
		if errno != 0 {
			reset();
			log.Printf("syscall faild with: %v", errno);
			panic("turning off kbd repeat failed, are you root?");
		}

		resetters = append(resetters, func() {
			var original kbd_repeat;
			// these values are based on experience
			// because the fetching of old values
			// didn't seem to work
			original.delay = 250;
			original.period = 33;

			syscall.RawSyscall(
				syscall.SYS_IOCTL,
				0,
				uintptr(0x4B52), // KDKBDREP  
				uintptr(unsafe.Pointer(&original)));
		});
    }
	return reset;
}

const (
	K_ESC = 1;
	K_a = 30;
	K_LEFT = 105;
	K_RIGHT = 106;
	K_UP = 103;
	K_DOWN = 108;
)

type Event struct {
	key int;
	pressed bool;
}

func pump_events() ([]Event) {
	res := []Event{};
	fd := os.Stdin.Fd();
	timeout := syscall.Timeval{0, 0};
	in_descs := syscall.FdSet{};
	in_descs.Bits[fd] = 1;

	num, err := syscall.Select(int(fd) + 1, &in_descs, nil, nil, &timeout);
	if err != nil {
		panic("Select error!");
	}
	// we actually got input
	if num > 0 {
		if in_descs.Bits[int(fd)] == 1 {
			var buffer [32]byte;
			rc, err := os.Stdin.Read(buffer[0:]);
			if err != nil {
				panic("error reading stdin!");
			}
			for i := 0; i < rc; i++ {
				c := buffer[i];
				pressed := c & 0x80 == 0;
				c &= 0x7f;
				res = append(res, Event{int(c), pressed});
			}
		}
	}
	return res;
}

func show(width, height int, w2, h2, w float64) {
	openvg.BackgroundColor("black")                           // Black background
	openvg.FillRGB(44, 100, 232, 1)                           // Big blue marble
	openvg.Circle(w2, 0, w)                                   // The "world"
	openvg.FillColor("white")                                 // White text
	openvg.TextMid(w2, h2, "hello, world", "serif", width/10) // Greetings 
}	


func fps(width, height int, elapsed time.Duration) {
	openvg.FillColor("white") ;
	s := "fps: ---";
	if elapsed > 0 {
		the_fps := 1.0 / (float64(elapsed) / float64(time.Second));
		s = fmt.Sprintf("fps: %.2f", the_fps);
	}
	openvg.Text(0.0, float64(height - 30), s, "mono", 20);
}



type World struct {
	now time.Time;
	elapsed time.Duration;
	width, height int;
	show_hud bool;
	pressed_keys map[int] bool;
}


func (world *World) Init(width, height int) {
	world.now = time.Now();
	world.width = width;
	world.height = height;
	world.pressed_keys = *new(map[int] bool);
}


func (world *World) Begin() {
	world.elapsed = time.Since(world.now);
	world.now = world.now.Add(world.elapsed);
	openvg.Start(world.width, world.height);
}


func (world *World) End(tie_to_fps float64) {
	if world.show_hud {
		fps(world.width, world.height, world.elapsed);
	}
	openvg.End();
	if tie_to_fps > 0 {
		period := time.Duration((1.0 / tie_to_fps) * float64(time.Second));
		frame_time := time.Since(world.now);
		period -= frame_time;
		if period > 0 {
			time.Sleep(period);
		}
	}
}

func (world *World) ProcessEvents(events []Event) {
	for i := 0; i < len(events); i++ {
		event := events[i];
		world.pressed_keys[event.key] = event.pressed;
	}
}


func (world *World) IsPressed(key int) bool {
	return world.pressed_keys[key];
}


func main() {
	width, height := openvg.Init() // OpenGL, etc initialization

	w2 := float64(width / 2);
	h2 := float64(height / 2);
	w := float64(width);

		
	stdin_fd := os.Stdin.Fd();
	f := test(stdin_fd, true);
	defer f();
	running := true;
	world := World{};
	world.show_hud = true;
	world.Init(width, height);
	for running {
		events := pump_events();
		world.ProcessEvents(events);
		world.Begin();
		if world.IsPressed(K_ESC) {
			running = false;
		}
		if world.IsPressed(K_LEFT) {
			w2 -= 1;
		}
		if world.IsPressed(K_RIGHT) {
			w2 += 1;
		}
		show(width, height, w2, h2, w);
		world.End(30.0);
	}
}