#include <string.h>

#include "fonts/visitor1.c"
#include "assets/assets.hh"

Fontinfo VisitorTypeface;

static const int MAXFONTPATH = 256;

// loadfont loads font path data
// derived from github.com/ajstarks/openvg/
// which 
// derived from http://web.archive.org/web/20070808195131/http://developer.hybrid.fi/font2openvg/renderFont.cpp.txt

Fontinfo AssetManager::loadfont(const int *Points,
		  const int *PointIndices,
		  const unsigned char *Instructions,
		  const int *InstructionIndices, const int *InstructionCounts, const int *adv, const short *cmap, int ng) const {

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
		VGPath path = _vg->vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_S_32,
					   1.0f / 65536.0f, 0.0f, 0, 0,
					   VG_PATH_CAPABILITY_ALL);
		f.Glyphs[i] = path;
		if (ic) {
			_vg->vgAppendPathData(path, ic, instructions, p);
		}
	}
	f.CharacterMap = cmap;
	f.GlyphAdvances = adv;
	f.Count = ng;
	return f;
}


void AssetManager::fonts_init() {
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



AssetManager::AssetManager(const OpenVGAdapter* vg) 
  : _vg(vg)
{
  fonts_init();
}


Fontinfo AssetManager::get_font() const {
  return VisitorTypeface;
}


VGfloat AssetManager::textWidth(char *s, Fontinfo f, int pointsize) const {
  int i;
  VGfloat tw = 0.0;
  VGfloat size = (VGfloat) pointsize;
  for (i = 0; i < (int)strlen(s); i++) {
    unsigned int character = (unsigned int)s[i];
    int glyph = f.CharacterMap[character];
    if (glyph == -1) {
      continue;	//glyph is undefined
    }
    tw += size * f.GlyphAdvances[glyph] / 65536.0f;
  }
  return tw;
}

void AssetManager::drawText(VGfloat x, VGfloat y, char *s, Fontinfo f, int pointsize) const {
  VGfloat size = (VGfloat) pointsize, xx = x, mm[9];
  int i;
  
  _vg->vgGetMatrix(mm);
  for (i = 0; i < (int)strlen(s); i++) {
    unsigned int character = (unsigned int)s[i];
    int glyph = f.CharacterMap[character];
    if (glyph == -1) {
      continue;	//glyph is undefined
    }
    VGfloat mat[9] = {
      size, 0.0f, 0.0f,
      0.0f, size, 0.0f,
      xx, y, 1.0f
    };
    _vg->vgLoadMatrix(mm);
    _vg->vgMultMatrix(mat);
    _vg->vgDrawPath(f.Glyphs[glyph], VG_FILL_PATH);
    xx += size * f.GlyphAdvances[glyph] / 65536.0f;
  }
  _vg->vgLoadMatrix(mm);
}



const PNGImageData& AssetManager::image(const fs::path &file) {
  if(_image_data.find(file) == _image_data.end()) {
    PNGImageData &image_data = _image_data[file];
    image_data.load(file);
  }
  return _image_data[file];
}


PNGImageData::PNGImageData() 
  : _raw(0)
  , _width(0)
  , _height(0)
{
}


PNGImageData::PNGImageData(const PNGImageData &original) {
  _width = original._width;
  _height = original._height;
  if(original._raw) {
    _raw = new std::vector<png::rgba_pixel>(*original._raw);
  } else {
    _raw = 0;
  }
}


void PNGImageData::load(const fs::path &file) {
  png::image< png::rgba_pixel > image(file.c_str());
  _width = image.get_width();
  _height = image.get_height();
  _raw = new std::vector< png::rgba_pixel >(_width * _height);
  
  for (size_t i = 0; i < _height; ++i) {
    std::copy(
	image.get_row(i).begin(), image.get_row(i).end(),
	_raw->begin() + i*_width);
  }
}


PNGImageData::~PNGImageData() {
  delete _raw;
}
