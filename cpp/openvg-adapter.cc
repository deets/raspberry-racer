#include "openvg-adapter.hh"

void OpenVGAdapter::setFillColor(VGfloat color[4]) {
  VGPaint fillPaint = this->vgCreatePaint();
  this->vgSetParameteri(fillPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
  this->vgSetParameterfv(fillPaint, VG_PAINT_COLOR, 4, color);
  this->vgSetPaint(fillPaint, VG_FILL_PATH);
  this->vgDestroyPaint(fillPaint);
}


void OpenVGAdapter::drawEllipse(VGfloat x, VGfloat y, VGfloat w, VGfloat h) {
  VGPath path = newPath();
  this->vguEllipse(path, x, y, w, h);
  this->vgDrawPath(path, VG_FILL_PATH | VG_STROKE_PATH);
  this->vgDestroyPath(path);
}


VGPath OpenVGAdapter::newPath() {
  return this->vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
}
