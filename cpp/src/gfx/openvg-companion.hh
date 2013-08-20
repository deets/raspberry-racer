#ifndef OPENVG_COMPANION_HH
#define OPENVG_COMPANION_HH

#include "common/non-copyable.hh"
#include "assets/assets.hh"
#include "gfx/openvg-adapter.hh"

class OpenVGCompanion : public NonCopyAble {

  const OpenVGAdapter &_vg;

public:
  OpenVGCompanion(const OpenVGAdapter &vg);
  virtual ~OpenVGCompanion();
  const OpenVGAdapter &vg() const { return _vg; }

  // the utility functions
  VGPath newPath() const;
  void setFillColor(VGfloat color[4]) const;
  void drawEllipse(VGfloat x, VGfloat y, VGfloat w, VGfloat h) const;
  void drawCircle(VGfloat x, VGfloat y, VGfloat r) const {
    drawEllipse(x, y, r, r);
  }

  void drawImage(const PNGImageData& image_data, VGint x, VGint y) const;

  void drawText(const Fontinfo& fi, VGfloat x, VGfloat y, char *s, int pointsize) const;
  VGfloat textWidth(const Fontinfo& fi, char *s, int pointsize) const;


};


#endif