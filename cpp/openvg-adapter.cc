#include <string.h>
#include "openvg-adapter.hh"

void OpenVGAdapter::setFillColor(VGfloat color[4]) const {
  VGPaint fillPaint = this->vgCreatePaint();
  this->vgSetParameteri(fillPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
  this->vgSetParameterfv(fillPaint, VG_PAINT_COLOR, 4, color);
  this->vgSetPaint(fillPaint, VG_FILL_PATH);
  this->vgDestroyPaint(fillPaint);
}


void OpenVGAdapter::drawEllipse(VGfloat x, VGfloat y, VGfloat w, VGfloat h) const {
  VGPath path = newPath();
  this->vguEllipse(path, x, y, w, h);
  this->vgDrawPath(path, VG_FILL_PATH | VG_STROKE_PATH);
  this->vgDestroyPath(path);
}


VGPath OpenVGAdapter::newPath() const {
  return this->vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
}

VGfloat OpenVGAdapter::textWidth(char *s, Fontinfo f, int pointsize) const {
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

void OpenVGAdapter::drawText(VGfloat x, VGfloat y, char *s, Fontinfo f, int pointsize) const {
  VGfloat size = (VGfloat) pointsize, xx = x, mm[9];
  int i;
  
  this->vgGetMatrix(mm);
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
    this->vgLoadMatrix(mm);
    this->vgMultMatrix(mat);
    this->vgDrawPath(f.Glyphs[glyph], VG_FILL_PATH);
    xx += size * f.GlyphAdvances[glyph] / 65536.0f;
  }
  this->vgLoadMatrix(mm);
}
