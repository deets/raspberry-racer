#ifndef OPENVG_ADAPTER_HH
#define OPENVG_ADAPTER_HH

#include <VG/openvg.h>
#include <VG/vgu.h>


class OpenVGAdapter {

public:

  // passing through of OpenVG calls
  virtual void vgSeti(VGParamType type, VGint value) const=0;
  virtual void vgSetf(VGParamType type, VGfloat value) const=0;
  virtual void vgSetfv(VGParamType type, VGint count, VGfloat* values) const=0;
  virtual void vgClear(VGint x, VGint y, VGint width, VGint height) const=0;
  virtual void vgLoadIdentity() const=0;
  virtual VGPaint vgCreatePaint() const=0;
  virtual void vgDestroyPaint(VGPaint paint) const=0;
  virtual void vgSetPaint(VGPaint paint, VGbitfield paintModes) const=0;
  virtual void vgSetParameteri(VGHandle object,
			       VGint paramType,
			       VGint value) const=0;

  virtual void vgSetParameteriv(VGHandle object,
				VGint paramType,
				VGint count,
				VGint * values) const=0;

  virtual void vgSetParameterf(VGHandle object,
                                 VGint paramType,
			       VGfloat value) const=0;

  virtual void vgSetParameterfv(VGHandle object,
				VGint paramType,
				VGint count,
				VGfloat * values) const=0;


  virtual void vgDrawPath(VGPath path, VGbitfield paintModes) const=0;
  virtual void vgDestroyPath(VGPath path) const=0;
  virtual VGUErrorCode vguEllipse(VGPath path,
				  VGfloat cx, VGfloat cy,
				  VGfloat width, VGfloat height) const=0;

  virtual VGPath vgCreatePath(VGint pathFormat,
			      VGPathDatatype datatype,
			      VGfloat scale,
			      VGfloat bias,
			      VGint segmentCapacityHint,
			      VGint coordCapacityHint,
			      VGbitfield capabilities) const=0;

  virtual void vgGetMatrix(VGfloat * m) const=0;
  virtual void vgLoadMatrix(VGfloat *m) const=0;
  virtual void vgMultMatrix(VGfloat *m) const=0;

  virtual void vgAppendPathData(VGPath dstPath,
				VGint numSegments,
				const VGubyte * pathSegments,
				const void * pathData) const=0;


  virtual VGImage vgCreateImage(VGImageFormat format,
				VGint width, VGint height,
				VGbitfield allowedQuality) const=0;

  virtual void vgDrawImage(VGImage) const=0;
  virtual void vgDestroyImage(VGImage) const=0;

  virtual void vgImageSubData(VGImage image,
			      const void * data, VGint dataStride,
			      VGImageFormat dataFormat,
			      VGint x, VGint y, VGint width, VGint height) const=0;


  virtual void vgSetPixels(VGint dx, VGint dy,
			   VGImage src, VGint sx, VGint sy,
			   VGint width, VGint height) const=0;


  virtual void vgTranslate(VGfloat x, VGfloat y) const=0;
  virtual void vgRotate(VGfloat deg) const=0;
};


#endif
