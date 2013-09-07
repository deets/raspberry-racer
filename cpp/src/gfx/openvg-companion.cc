#include <string.h>
#include "gfx/openvg-companion.hh"

namespace rracer {

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


  void OpenVGCompanion::drawImage(const PNGImageData& image_data) const {
    VGint width = image_data.width();
    VGint height = image_data.height();
    VGImage img = _vg.vgCreateImage(
	VG_sRGBA_8888, 
	width, height,
	VG_IMAGE_QUALITY_BETTER);
    _vg.vgImageSubData(img, image_data.data(), image_data.stride(), VG_sRGBA_8888, 0, 0, width, height);
    _vg.vgSeti(VG_IMAGE_MODE, VG_DRAW_IMAGE_NORMAL);
    VGPaint paint = _vg.vgCreatePaint();
    _vg.vgSetParameteri(paint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
    VGfloat white[4] = {1.0, 1.0, 1.0, 1.0};
    _vg.vgSetParameterfv(paint, VG_PAINT_COLOR, 4, white);
  
    _vg.vgSetPaint(paint, VG_FILL_PATH);
    _vg.vgDrawImage(img);
    _vg.vgDestroyPaint(paint);
    _vg.vgDestroyImage(img);
  }


  VGfloat OpenVGCompanion::textWidth(const Fontinfo& fi, char *s, int pointsize) const {
    int i;
    VGfloat tw = 0.0;
    VGfloat size = (VGfloat) pointsize;
    for (i = 0; i < (int)strlen(s); i++) {
      unsigned int character = (unsigned int)s[i];
      int glyph = fi.CharacterMap[character];
      if (glyph == -1) {
	continue;	//glyph is undefined
      }
      tw += size * fi.GlyphAdvances[glyph] / 65536.0f;
    }
    return tw;
  }

  void OpenVGCompanion::drawText(const Fontinfo& fi, VGfloat x, VGfloat y, char *s, int pointsize) const {
    VGfloat size = (VGfloat) pointsize, xx = x, mm[9];
    int i;
  
    _vg.vgGetMatrix(mm);
    for (i = 0; i < (int)strlen(s); i++) {
      unsigned int character = (unsigned int)s[i];
      int glyph = fi.CharacterMap[character];
      if (glyph == -1) {
	continue;	//glyph is undefined
      }
      VGfloat mat[9] = {
	size, 0.0f, 0.0f,
	0.0f, size, 0.0f,
	xx, y, 1.0f
      };
      _vg.vgLoadMatrix(mm);
      _vg.vgMultMatrix(mat);
      _vg.vgDrawPath(fi.Glyphs[glyph], VG_FILL_PATH);
      xx += size * fi.GlyphAdvances[glyph] / 65536.0f;
    }
    _vg.vgLoadMatrix(mm);
  }


  void OpenVGCompanion::move_to(VGPath p, Vector point, VGPathAbsRel absrel) const {
    VGfloat coords[] = { point[0], point[1] };
    VGubyte cmd = VG_MOVE_TO | absrel;
    _vg.vgAppendPathData(p, 1, &cmd, coords);
  }


  void OpenVGCompanion::line_to(VGPath p, Vector point, VGPathAbsRel absrel) const {
    VGfloat coords[] = { point[0], point[1] };
    VGubyte cmd = VG_LINE_TO | absrel;
    _vg.vgAppendPathData(p, 1, &cmd, coords);
  }


  void OpenVGCompanion::close(VGPath p) const {
    VGubyte cmd = VG_CLOSE_PATH;
    _vg.vgAppendPathData(p, 1, &cmd, NULL);
  }

  void OpenVGCompanion::arc(VGPath p, VGPathSegment kind, Vector to, VGfloat rh, VGfloat rv, VGfloat rot, VGPathAbsRel absrel) const {
    VGubyte cmd = kind | absrel;
    VGfloat params[] = { rh,rv,rot, to[0], to[1] };
    _vg.vgAppendPathData(p, 1, &cmd, params);
  }


  void OpenVGCompanion::draw_rect(const Rect& r) const {
    VGPath p = newPath();
    move_to(p, Vector(r.left(), r.bottom()), VG_ABSOLUTE);
    line_to(p, Vector(r.right(), r.bottom()), VG_ABSOLUTE);
    line_to(p, Vector(r.right(), r.top()), VG_ABSOLUTE);
    line_to(p, Vector(r.left(), r.top()), VG_ABSOLUTE);
    line_to(p, Vector(r.left(), r.bottom()), VG_ABSOLUTE);
    close(p);
    _vg.vgDrawPath(p, VG_STROKE_PATH);
    _vg.vgDestroyPath(p);
  }


  MatrixStacker::MatrixStacker(const OpenVGCompanion& vgc)
    : _vgc(vgc)
  {
    _vgc.vg().vgGetMatrix(_m);
  }
  MatrixStacker::~MatrixStacker() {
    _vgc.vg().vgLoadMatrix(_m);
  }


  PaintSetter::PaintSetter(const OpenVGCompanion& vgc, const VGfloat* color, VGbitfield paint_mode) 
    : _vgc(vgc)
  {
    _paint = _vgc.vg().vgCreatePaint();
    _vgc.vg().vgSetParameteri(_paint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
    _vgc.vg().vgSetParameterfv(_paint, VG_PAINT_COLOR, 4, color);
    _vgc.vg().vgSetPaint(_paint, paint_mode);
  }

  PaintSetter::~PaintSetter() {
    _vgc.vg().vgDestroyPaint(_paint);
  }

}; // ns::racer
