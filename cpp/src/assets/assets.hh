#ifndef ASSETS_HH
#define ASSETS_HH


#include <map>
#include <boost/filesystem.hpp>

#include <VG/openvg.h>
#include <png++/png.hpp>

#include "json/json-forwards.h"

#include "gfx/openvg-adapter.hh"
#include "common/non-copyable.hh"

namespace fs = boost::filesystem;

using namespace std;

typedef struct {
	const short *CharacterMap;
	const int *GlyphAdvances;
	int Count;
	VGPath Glyphs[256];
} Fontinfo;



class PNGImageData {
  size_t _width, _height;
  std::vector< png::rgba_pixel > *_raw;

public:
  PNGImageData();
  PNGImageData(const PNGImageData &original);
  virtual ~PNGImageData();

  void load(const fs::path &file);

  VGint width() const { return _width; }
  VGint height() const { return _height; }
  VGint stride() const { return _width * 4; /* sizeof rgba-pixel */ }
  void *data() const { return (void*)&(*_raw)[0]; }
};


class AssetManager : public NonCopyAble {


public:

  AssetManager(const OpenVGAdapter& vg);
  AssetManager(const OpenVGAdapter& vg, const fs::path base);

  const Fontinfo& font();
  const PNGImageData& image(const fs::path&);
  Json::Value json(const fs::path&);

private:

  bool _fonts_initialized;
  map<fs::path, PNGImageData> _image_data;

  const fs::path _base;
  const OpenVGAdapter& _vg;

  Fontinfo loadfont(const int *Points,
		    const int *PointIndices,
		    const unsigned char *Instructions,
		    const int *InstructionIndices, const int *InstructionCounts, const int *adv, const short *cmap, int ng) const;
  void fonts_init();

  /**
   * Resolves the passed path relative
   * to our own _base, if set
   */
  const fs::path resolve_path(const fs::path p);

};


#endif
