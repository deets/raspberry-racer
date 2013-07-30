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


void RPiWindowAdapter::vgSetf (VGParamType type, VGfloat value) {
  ::vgSetf(type, value);
}


void RPiWindowAdapter::vgClear(VGint x, VGint y, VGint width, VGint height) {
  ::vgClear(x, y, width, height);
}


void RPiWindowAdapter::vgLoadIdentity() {
  ::vgLoadIdentity();
}


VGPaint RPiWindowAdapter::vgCreatePaint() {
  return ::vgCreatePaint();
}


void RPiWindowAdapter::vgDestroyPaint(VGPaint paint) {
  ::vgDestroyPaint(paint);
}


void RPiWindowAdapter::vgSetPaint(VGPaint paint, VGbitfield paintModes) {
  ::vgSetPaint(paint, paintModes);
}


void RPiWindowAdapter::vgSetParameteri(VGHandle object,
					       VGint paramType,
					       VGint value) {
  ::vgSetParameteri(object, paramType, value);
}


void RPiWindowAdapter::vgSetParameteriv(VGHandle object,
					VGint paramType,
					VGint count,
					VGint *values) {
  ::vgSetParameteriv(object, paramType, count, values);
}


void RPiWindowAdapter::vgSetParameterf(VGHandle object,
					       VGint paramType,
					       VGfloat value) {
  ::vgSetParameterf(object, paramType, value);
}


void RPiWindowAdapter::vgSetParameterfv(VGHandle object,
					VGint paramType,
					VGint count,
					VGfloat *values) {
  ::vgSetParameterfv(object, paramType, count, values);
}



void RPiWindowAdapter::vgDrawPath(VGPath path, VGbitfield paintModes) {
  ::vgDrawPath(path, paintModes);
}


void RPiWindowAdapter::vgDestroyPath(VGPath path) {
  ::vgDestroyPath(path);
}


VGUErrorCode RPiWindowAdapter::vguEllipse(VGPath path,
						  VGfloat cx, VGfloat cy,
						  VGfloat width, VGfloat height) {
  return ::vguEllipse(path, cx, cy, width, height);
}


VGPath RPiWindowAdapter::vgCreatePath(VGint pathFormat,
				      VGPathDatatype datatype,
				      VGfloat scale, VGfloat bias,
				      VGint segmentCapacityHint,
				      VGint coordCapacityHint,
				      VGbitfield capabilities) {
  return ::vgCreatePath(pathFormat, datatype, scale, bias, segmentCapacityHint, coordCapacityHint, capabilities);
}
