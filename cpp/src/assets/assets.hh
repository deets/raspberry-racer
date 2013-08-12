#ifndef ASSETS_HH
#define ASSETS_HH


#include <map>
#include <boost/filesystem.hpp>

#include <VG/openvg.h>
#include "openvg-adapter.hh"
#include "posix-adapter.hh"
#include "common/non-copyable.hh"

namespace fs = boost::filesystem;

using namespace std;

typedef struct {
	const short *CharacterMap;
	const int *GlyphAdvances;
	int Count;
	VGPath Glyphs[256];
} Fontinfo;


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

  VGImage image(const fs::path&);

private:

  map<fs::path, VGImage> _images;
};


#endif
