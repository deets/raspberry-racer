#include <string.h>
#include "gfx/openvg-companion.hh"

OpenVGCompanion::OpenVGCompanion(const OpenVGAdapter &vg) :
  _vg(vg)
{
}


OpenVGCompanion::~OpenVGCompanion() {}

void OpenVGCompanion::setFillColor(VGfloat color[4]) const {
  VGPaint fillPaint = _vg.vgCreatePaint();
  _vg.vgSetParameteri(fillPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
  _vg.vgSetParameterfv(fillPaint, VG_PAINT_COLOR, 4, color);
  _vg.vgSetPaint(fillPaint, VG_FILL_PATH);
  _vg.vgDestroyPaint(fillPaint);
}


void OpenVGCompanion::drawEllipse(VGfloat x, VGfloat y, VGfloat w, VGfloat h) const {
  VGPath path = newPath();
  _vg.vguEllipse(path, x, y, w, h);
  _vg.vgDrawPath(path, VG_FILL_PATH | VG_STROKE_PATH);
  _vg.vgDestroyPath(path);
}


VGPath OpenVGCompanion::newPath() const {
  return _vg.vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
}
