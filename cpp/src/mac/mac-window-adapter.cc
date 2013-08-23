#include "mac/mac-window-adapter.hh"



MacWindowAdapter::MacWindowAdapter(int width, int height)
  : _width(width)
  , _height(height) 
{
}


MacWindowAdapter::~MacWindowAdapter() {
}

  // WindowAdapter implementation
pair<int, int> MacWindowAdapter::window_dimensions() {
  return make_pair(_width, _height);
}


void MacWindowAdapter::start() {
}


void MacWindowAdapter::end() {

}

  // OpenVGAdapter implementation
void MacWindowAdapter::vgSetf (VGParamType type, VGfloat value) const {
  ::vgSetf(type, value);
}


void MacWindowAdapter::vgClear(VGint x, VGint y, VGint width, VGint height) const {
  ::vgClear(x, y, width, height);
}


void MacWindowAdapter::vgLoadIdentity() const {
  ::vgLoadIdentity();
}


VGPaint MacWindowAdapter::vgCreatePaint() const {
  return ::vgCreatePaint();
}


void MacWindowAdapter::vgDestroyPaint(VGPaint paint) const {
  ::vgDestroyPaint(paint);
}


void MacWindowAdapter::vgSetPaint(VGPaint paint, VGbitfield paintModes) const {
  ::vgSetPaint(paint, paintModes);
}


void MacWindowAdapter::vgSetParameteri(VGHandle object,
				       VGint paramType,
				       VGint value) const {
  ::vgSetParameteri(object, paramType, value);
}


void MacWindowAdapter::vgSetParameteriv(VGHandle object,
					VGint paramType,
					VGint count,
					VGint * values) const {
  ::vgSetParameteriv(object, paramType, count, values);
}


void MacWindowAdapter::vgSetParameterf(VGHandle object,
				       VGint paramType,
				       VGfloat value) const {
  ::vgSetParameterf(object, paramType, value);
}


void MacWindowAdapter::vgSetParameterfv(VGHandle object,
				VGint paramType,
				VGint count,
					VGfloat * values) const {
  ::vgSetParameterfv(object, paramType, count, values);
}





void MacWindowAdapter::vgDrawPath(VGPath path, VGbitfield paintModes) const {
  ::vgDrawPath(path, paintModes);
}


void MacWindowAdapter::vgDestroyPath(VGPath path) const {
  ::vgDestroyPath(path);
}


VGUErrorCode MacWindowAdapter::vguEllipse(VGPath path,
					  VGfloat cx, VGfloat cy,
					  VGfloat width, VGfloat height) const {
  return ::vguEllipse(path, cx, cy, width, height);
}


VGPath MacWindowAdapter::vgCreatePath(VGint pathFormat,
				      VGPathDatatype datatype,
				      VGfloat scale, VGfloat bias,
				      VGint segmentCapacityHint,
				      VGint coordCapacityHint,
				      VGbitfield capabilities) const {
  return ::vgCreatePath(pathFormat, datatype, scale, bias, segmentCapacityHint, coordCapacityHint, capabilities);
}


void MacWindowAdapter::vgGetMatrix(VGfloat * m) const {
  ::vgGetMatrix(m);
}

void MacWindowAdapter::vgLoadMatrix(VGfloat *m) const {
  ::vgLoadMatrix(m);
}

void MacWindowAdapter::vgMultMatrix(VGfloat *m) const {
  ::vgMultMatrix(m);
}


void MacWindowAdapter::vgAppendPathData(VGPath dstPath,
					VGint numSegments,
					const VGubyte * pathSegments,
					const void * pathData)  const {
  ::vgAppendPathData(dstPath, numSegments, pathSegments, pathData);
}


VGImage MacWindowAdapter::vgCreateImage(VGImageFormat format,
					VGint width, VGint height,
					VGbitfield allowedQuality) const {
  return ::vgCreateImage(format, width, height, allowedQuality);
}



void MacWindowAdapter::vgImageSubData(VGImage image,
				      const void * data, VGint dataStride,
				      VGImageFormat dataFormat,
				      VGint x, VGint y, VGint width, VGint height) const {
  ::vgImageSubData(image, data, dataStride, dataFormat, x, y, width, height);
}



void MacWindowAdapter::vgSetPixels(VGint dx, VGint dy,
			   VGImage src, VGint sx, VGint sy,
				   VGint width, VGint height) const {
  ::vgSetPixels(dx, dy, src, sx, sy, width, height);
}



void MacWindowAdapter::vgDestroyImage(VGImage img) const {
  ::vgDestroyImage(img);
}


