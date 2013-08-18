#ifndef OPENVG_COMPANION_HH
#define OPENVG_COMPANION_HH

#include "common/non-copyable.hh"
#include "gfx/openvg-adapter.hh"

class OpenVGCompanion : public NonCopyAble {

  const OpenVGAdapter &_vg;

public:
  OpenVGCompanion(const OpenVGAdapter &vg);
  virtual ~OpenVGCompanion();

  // utility functions
  VGPath newPath() const;
  void setFillColor(VGfloat color[4]) const;
  void drawEllipse(VGfloat x, VGfloat y, VGfloat w, VGfloat h) const;
  void drawCircle(VGfloat x, VGfloat y, VGfloat r) const {
    drawEllipse(x, y, r, r);
  }



};


#endif
