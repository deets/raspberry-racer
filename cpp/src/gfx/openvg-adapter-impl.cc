#include "openvg-adapter-impl.hh"



// OpenVGAdapter implementation
void OpenVGAdapterImpl::vgSetf (VGParamType type, VGfloat value) const {
  ::vgSetf(type, value);
}


void OpenVGAdapterImpl::vgSetfv(VGParamType type, VGint count, VGfloat* value) const {
  ::vgSetfv(type, count, value);
}


void OpenVGAdapterImpl::vgSeti(VGParamType type, VGint value) const {
  ::vgSeti(type, value);
}


void OpenVGAdapterImpl::vgClear(VGint x, VGint y, VGint width, VGint height) const {
  ::vgClear(x, y, width, height);
}


void OpenVGAdapterImpl::vgLoadIdentity() const {
  ::vgLoadIdentity();
}


VGPaint OpenVGAdapterImpl::vgCreatePaint() const {
  return ::vgCreatePaint();
}


void OpenVGAdapterImpl::vgDestroyPaint(VGPaint paint) const {
  ::vgDestroyPaint(paint);
}


void OpenVGAdapterImpl::vgSetPaint(VGPaint paint, VGbitfield paintModes) const {
  ::vgSetPaint(paint, paintModes);
}


void OpenVGAdapterImpl::vgSetParameteri(VGHandle object,
				       VGint paramType,
				       VGint value) const {
  ::vgSetParameteri(object, paramType, value);
}


void OpenVGAdapterImpl::vgSetParameteriv(VGHandle object,
					VGint paramType,
					VGint count,
					VGint * values) const {
  ::vgSetParameteriv(object, paramType, count, values);
}


void OpenVGAdapterImpl::vgSetParameterf(VGHandle object,
				       VGint paramType,
				       VGfloat value) const {
  ::vgSetParameterf(object, paramType, value);
}


void OpenVGAdapterImpl::vgSetParameterfv(VGHandle object,
				VGint paramType,
				VGint count,
					VGfloat * values) const {
  ::vgSetParameterfv(object, paramType, count, values);
}





void OpenVGAdapterImpl::vgDrawPath(VGPath path, VGbitfield paintModes) const {
  ::vgDrawPath(path, paintModes);
}


void OpenVGAdapterImpl::vgDestroyPath(VGPath path) const {
  ::vgDestroyPath(path);
}


VGUErrorCode OpenVGAdapterImpl::vguEllipse(VGPath path,
					  VGfloat cx, VGfloat cy,
					  VGfloat width, VGfloat height) const {
  return ::vguEllipse(path, cx, cy, width, height);
}


VGPath OpenVGAdapterImpl::vgCreatePath(VGint pathFormat,
				      VGPathDatatype datatype,
				      VGfloat scale, VGfloat bias,
				      VGint segmentCapacityHint,
				      VGint coordCapacityHint,
				      VGbitfield capabilities) const {
  return ::vgCreatePath(pathFormat, datatype, scale, bias, segmentCapacityHint, coordCapacityHint, capabilities);
}


void OpenVGAdapterImpl::vgGetMatrix(VGfloat * m) const {
  ::vgGetMatrix(m);
}

void OpenVGAdapterImpl::vgLoadMatrix(VGfloat *m) const {
  ::vgLoadMatrix(m);
}

void OpenVGAdapterImpl::vgMultMatrix(VGfloat *m) const {
  ::vgMultMatrix(m);
}


void OpenVGAdapterImpl::vgAppendPathData(VGPath dstPath,
					VGint numSegments,
					const VGubyte * pathSegments,
					const void * pathData)  const {
  ::vgAppendPathData(dstPath, numSegments, pathSegments, pathData);
}


VGImage OpenVGAdapterImpl::vgCreateImage(VGImageFormat format,
					VGint width, VGint height,
					VGbitfield allowedQuality) const {
  return ::vgCreateImage(format, width, height, allowedQuality);
}



void OpenVGAdapterImpl::vgImageSubData(VGImage image,
				      const void * data, VGint dataStride,
				      VGImageFormat dataFormat,
				      VGint x, VGint y, VGint width, VGint height) const {
  ::vgImageSubData(image, data, dataStride, dataFormat, x, y, width, height);
}



void OpenVGAdapterImpl::vgSetPixels(VGint dx, VGint dy,
			   VGImage src, VGint sx, VGint sy,
				   VGint width, VGint height) const {
  ::vgSetPixels(dx, dy, src, sx, sy, width, height);
}


void OpenVGAdapterImpl::vgDrawImage(VGImage img) const {
  ::vgDrawImage(img);
}


void OpenVGAdapterImpl::vgDestroyImage(VGImage img) const {
  ::vgDestroyImage(img);
}


void OpenVGAdapterImpl::vgTranslate(VGfloat x, VGfloat y) const {
  ::vgTranslate(x, y);
}


void OpenVGAdapterImpl::vgRotate(VGfloat deg) const {
  ::vgRotate(deg);
}
