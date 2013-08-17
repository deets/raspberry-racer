#ifndef ASSETS_HH
#define ASSETS_HH


#include <map>
#include <boost/filesystem.hpp>

#include <VG/openvg.h>
#include <png++/png.hpp>

#include "openvg-adapter.hh"
#include "common/non-copyable.hh"

namespace fs = boost::filesystem;

using namespace std;

typedef struct {
	const short *CharacterMap;
	const int *GlyphAdvances;
	int Count;
	VGPath Glyphs[256];
} Fontinfo;


struct ImageInfo {
  const VGImage image;
  size_t width;
  size_t height;
};


class PNGImageData {
  size_t _width, _height;
  std::vector< png::rgba_pixel > *_raw;

public:
  PNGImageData();
  PNGImageData(const PNGImageData &original) {
  }

  virtual ~PNGImageData();

  void load(const fs::path &file);

  VGint width() const { return _width; }
  VGint height() const { return _height; }
  VGint stride() const { return _width * 4; /* sizeof rgba-pixel */ }
  void *data() const { return (void*)&(*_raw)[0]; }
};


class AssetManager : public NonCopyAble {

  const OpenVGAdapter* _vg;

  Fontinfo loadfont(const int *Points,
		    const int *PointIndices,
		    const unsigned char *Instructions,
		    const int *InstructionIndices, const int *InstructionCounts, const int *adv, const short *cmap, int ng) const;
  void fonts_init();

public:

  AssetManager(const OpenVGAdapter* vg);
  // TODO: naming convention
  Fontinfo get_font() const;

  void drawText(VGfloat x, VGfloat y, char *s, Fontinfo f, int pointsize) const;
  VGfloat textWidth(char *s, Fontinfo f, int pointsize) const;

  ImageInfo image(const fs::path&);

private:

  map<fs::path, PNGImageData> _image_data;
};


#endif
