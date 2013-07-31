#ifndef FONTS_HH
#define FONTS_HH
#include <VG/openvg.h>

typedef struct {
	const short *CharacterMap;
	const int *GlyphAdvances;
	int Count;
	VGPath Glyphs[256];
} Fontinfo;


void fonts_init();
Fontinfo get_font();

#endif
