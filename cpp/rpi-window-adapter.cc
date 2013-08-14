#include <assert.h>
#include <bcm_host.h>

#include "rpi-window-adapter.hh"



using namespace std;

RPiWindowAdapter::RPiWindowAdapter()
{
  ::bcm_host_init();
  ::memset(&_state, 0, sizeof(_state));
  ::oglinit(&_state);
  ::graphics_get_display_size(0, &_width, &_height);
}


RPiWindowAdapter::~RPiWindowAdapter() {
  ::bcm_host_deinit();
}


pair<int, int> RPiWindowAdapter::window_dimensions() {
  return make_pair((int)_width, (int)_height);
}

void RPiWindowAdapter::start() {
  VGfloat color[4] = { 255, 255, 255, 1 };
  ::vgSetfv(VG_CLEAR_COLOR, 4, color);
  ::vgClear(0, 0, _width, _height);
  color[0] = 0, color[1] = 0, color[2] = 0;
  //setfill(color);
  //setstroke(color);
  //StrokeWidth(0);
  ::vgLoadIdentity();
}

void RPiWindowAdapter::end() {
  assert(::vgGetError() == VG_NO_ERROR);
  ::eglSwapBuffers(_state.display, _state.surface);
  assert(::eglGetError() == EGL_SUCCESS);
}


void RPiWindowAdapter::vgSetf (VGParamType type, VGfloat value) const {
  ::vgSetf(type, value);
}


void RPiWindowAdapter::vgClear(VGint x, VGint y, VGint width, VGint height) const {
  ::vgClear(x, y, width, height);
}


void RPiWindowAdapter::vgLoadIdentity() const {
  ::vgLoadIdentity();
}


VGPaint RPiWindowAdapter::vgCreatePaint() const {
  return ::vgCreatePaint();
}


void RPiWindowAdapter::vgDestroyPaint(VGPaint paint) const {
  ::vgDestroyPaint(paint);
}


void RPiWindowAdapter::vgSetPaint(VGPaint paint, VGbitfield paintModes) const {
  ::vgSetPaint(paint, paintModes);
}


void RPiWindowAdapter::vgSetParameteri(VGHandle object,
					       VGint paramType,
					       VGint value) const {
  ::vgSetParameteri(object, paramType, value);
}


void RPiWindowAdapter::vgSetParameteriv(VGHandle object,
					VGint paramType,
					VGint count,
					VGint *values) const {
  ::vgSetParameteriv(object, paramType, count, values);
}


void RPiWindowAdapter::vgSetParameterf(VGHandle object,
					       VGint paramType,
					       VGfloat value) const {
  ::vgSetParameterf(object, paramType, value);
}


void RPiWindowAdapter::vgSetParameterfv(VGHandle object,
					VGint paramType,
					VGint count,
					VGfloat *values) const {
  ::vgSetParameterfv(object, paramType, count, values);
}



void RPiWindowAdapter::vgDrawPath(VGPath path, VGbitfield paintModes) const {
  ::vgDrawPath(path, paintModes);
}


void RPiWindowAdapter::vgDestroyPath(VGPath path) const {
  ::vgDestroyPath(path);
}


VGUErrorCode RPiWindowAdapter::vguEllipse(VGPath path,
						  VGfloat cx, VGfloat cy,
						  VGfloat width, VGfloat height) const {
  return ::vguEllipse(path, cx, cy, width, height);
}


VGPath RPiWindowAdapter::vgCreatePath(VGint pathFormat,
				      VGPathDatatype datatype,
				      VGfloat scale, VGfloat bias,
				      VGint segmentCapacityHint,
				      VGint coordCapacityHint,
				      VGbitfield capabilities) const {
  return ::vgCreatePath(pathFormat, datatype, scale, bias, segmentCapacityHint, coordCapacityHint, capabilities);
}


void RPiWindowAdapter::vgGetMatrix(VGfloat * m) const {
  ::vgGetMatrix(m);
}


void RPiWindowAdapter::vgLoadMatrix(VGfloat *m) const {
  ::vgLoadMatrix(m);
}


void RPiWindowAdapter::vgMultMatrix(VGfloat *m) const {
  ::vgMultMatrix(m);
}


void RPiWindowAdapter::vgAppendPathData(VGPath dstPath,
					VGint numSegments,
					const VGubyte * pathSegments,
					const void * pathData)  const {
  ::vgAppendPathData(dstPath, numSegments, pathSegments, pathData);
}


VGImage RPiWindowAdapter::vgCreateImage(VGImageFormat format,
					VGint width, VGint height,
					VGbitfield allowedQuality) const {
  return ::vgCreateImage(format, width, height, allowedQuality);
}


void RPiWindowAdapter::vgImageSubData(VGImage image,
				      const void * data, VGint dataStride,
				      VGImageFormat dataFormat,
				      VGint x, VGint y, VGint width, VGint height) const {
  ::vgImageSubData(image, data, dataStride, dataFormat, x, y, width, height);
}


void RPiWindowAdapter::vgSetPixels(VGint dx, VGint dy,
				   VGImage src, VGint sx, VGint sy,
				   VGint width, VGint height) const {
  ::vgSetPixels(dx, dy, src, sx, sy, width, height);
}
