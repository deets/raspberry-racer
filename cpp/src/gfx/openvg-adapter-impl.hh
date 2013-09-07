#ifndef OPENVG_ADAPTER_IMPL_HH
#define OPENVG_ADAPTER_IMPL_HH

#include "gfx/openvg-adapter.hh"

class OpenVGAdapterImpl : public OpenVGAdapter {

public:

  // OpenVGAdapter implementation
  virtual void vgSetf(VGParamType type, VGfloat value) const;
  virtual void vgSetfv(VGParamType type, VGint count, const VGfloat* value) const;
  virtual void vgSeti(VGParamType type, VGint value) const;
  virtual void vgClear(VGint x, VGint y, VGint width, VGint height) const;
  virtual void vgLoadIdentity() const;
  virtual VGPaint vgCreatePaint() const;
  virtual void vgDestroyPaint(VGPaint paint) const;
  virtual void vgSetPaint(VGPaint paint, VGbitfield paintModes) const;
  virtual void vgSetParameteri(VGHandle object,
			       VGint paramType,
			       VGint value) const;

  virtual void vgSetParameteriv(VGHandle object,
				VGint paramType,
				VGint count,
				const VGint* values) const;

  virtual void vgSetParameterf(VGHandle object,
			       VGint paramType,
			       VGfloat value) const;

  virtual void vgSetParameterfv(VGHandle object,
				VGint paramType,
				VGint count,
				const VGfloat* values) const;


  virtual void vgDrawPath(VGPath path, VGbitfield paintModes) const;
  virtual void vgDestroyPath(VGPath path) const;
  virtual VGUErrorCode vguEllipse(VGPath path,
				  VGfloat cx, VGfloat cy,
				  VGfloat width, VGfloat height) const;

  virtual VGPath vgCreatePath(VGint pathFormat,
			      VGPathDatatype datatype,
			      VGfloat scale, VGfloat bias,
			      VGint segmentCapacityHint,
			      VGint coordCapacityHint,
			      VGbitfield capabilities) const;

  virtual void vgGetMatrix(VGfloat * m) const;
  virtual void vgLoadMatrix(VGfloat *m) const;
  virtual void vgMultMatrix(VGfloat *m) const;

  virtual void vgAppendPathData(VGPath dstPath,
				VGint numSegments,
				const VGubyte * pathSegments,
				const void * pathData)  const;

  virtual VGImage vgCreateImage(VGImageFormat format,
				VGint width, VGint height,
				VGbitfield allowedQuality) const;


  virtual void vgImageSubData(VGImage image,
			      const void * data, VGint dataStride,
			      VGImageFormat dataFormat,
			      VGint x, VGint y, VGint width, VGint height) const;


  virtual void vgSetPixels(VGint dx, VGint dy,
			   VGImage src, VGint sx, VGint sy,
			   VGint width, VGint height) const;

  virtual void vgDrawImage(VGImage) const;

  virtual void vgDestroyImage(VGImage) const;

  virtual void vgTranslate(VGfloat x, VGfloat y) const;

  virtual void vgRotate(VGfloat deg) const;



};

#endif
