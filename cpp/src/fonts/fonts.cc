#include <string.h>
#include "fonts/visitor1.c"
#include "fonts/fonts.hh"

Fontinfo VisitorTypeface;
static const int MAXFONTPATH = 256;

// loadfont loads font path data
// derived from github.com/ajstarks/openvg/
// which 
// derived from http://web.archive.org/web/20070808195131/http://developer.hybrid.fi/font2openvg/renderFont.cpp.txt

Fontinfo loadfont(const int *Points,
		  const int *PointIndices,
		  const unsigned char *Instructions,
		  const int *InstructionIndices, const int *InstructionCounts, const int *adv, const short *cmap, int ng) {

	Fontinfo f;
	int i;

	memset(f.Glyphs, 0, MAXFONTPATH * sizeof(VGPath));
	if (ng > MAXFONTPATH) {
		return f;
	}
	for (i = 0; i < ng; i++) {
		const int *p = &Points[PointIndices[i] * 2];
		const unsigned char *instructions = &Instructions[InstructionIndices[i]];
		int ic = InstructionCounts[i];
		VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_S_32,
					   1.0f / 65536.0f, 0.0f, 0, 0,
					   VG_PATH_CAPABILITY_ALL);
		f.Glyphs[i] = path;
		if (ic) {
			vgAppendPathData(path, ic, instructions, p);
		}
	}
	f.CharacterMap = cmap;
	f.GlyphAdvances = adv;
	f.Count = ng;
	return f;
}


void fonts_init() {
  VisitorTypeface = loadfont(
      Visitor_glyphPoints,
      Visitor_glyphPointIndices,
      Visitor_glyphInstructions,
      Visitor_glyphInstructionIndices,
      Visitor_glyphInstructionCounts,
      Visitor_glyphAdvances, 
      Visitor_characterMap, 
      Visitor_glyphCount
  );
}  

Fontinfo get_font() {
  return VisitorTypeface;
}
