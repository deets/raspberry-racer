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
		VGPath path = _vg.vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_S_32,
					   1.0f / 65536.0f, 0.0f, 0, 0,
					   VG_PATH_CAPABILITY_ALL);
		f.Glyphs[i] = path;
		if (ic) {
			_vg.vgAppendPathData(path, ic, instructions, p);
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



AssetManager::AssetManager(const OpenVGAdapter& vg) 
  : _vg(vg)
{
  fonts_init();
}


AssetManager::AssetManager(const OpenVGAdapter& vg, const fs::path base) 
  : _vg(vg)
  , _base(base)
{
  fonts_init();
}


const fs::path AssetManager::resolve_path(const fs::path p) {
  if(_base.empty()) {
    return p;
  } else {
    return _base / p;
  }
}

const Fontinfo& AssetManager::get_font() const {
  return VisitorTypeface;
}


const PNGImageData& AssetManager::image(const fs::path &file) {
  const fs::path rfile = resolve_path(file);
  if(_image_data.find(rfile) == _image_data.end()) {
    PNGImageData &image_data = _image_data[rfile];
    image_data.load(rfile);
  }
  return _image_data[rfile];
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
    // the image is flipped here
    std::copy(
	image.get_row(_height - i - 1).begin(), image.get_row(_height - i - 1).end(),
	_raw->begin() + i*_width);
  }
}


PNGImageData::~PNGImageData() {
  delete _raw;
}
