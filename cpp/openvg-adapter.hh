#ifndef OPENVG_ADAPTER_HH
#define OPENVG_ADAPTER_HH

#include <VG/openvg.h>
#include <VG/vgu.h>

class OpenVGAdapter {

public:
  // utility functions
  VGPath newPath();
  void setFillColor(VGfloat color[4]);
  void drawEllipse(VGfloat x, VGfloat y, VGfloat w, VGfloat h);
  void drawCircle(VGfloat x, VGfloat y, VGfloat r) {
    drawEllipse(x, y, r, r);
  }


  // passing through of OpenVG calls
  virtual void vgSetf(VGParamType type, VGfloat value)=0;
  virtual void vgClear(VGint x, VGint y, VGint width, VGint height)=0;
  virtual void vgLoadIdentity()=0;
  virtual VGPaint vgCreatePaint()=0;
  virtual void vgDestroyPaint(VGPaint paint)=0;
  virtual void vgSetPaint(VGPaint paint, VGbitfield paintModes)=0;
  virtual void vgSetParameteri(VGHandle object,
			       VGint paramType,
			       VGint value)=0;

  virtual void vgSetParameteriv(VGHandle object,
				VGint paramType,
				VGint count,
				VGint * values)=0;

  virtual void vgSetParameterf(VGHandle object,
                                 VGint paramType,
			       VGfloat value)=0;

  virtual void vgSetParameterfv(VGHandle object,
				VGint paramType,
				VGint count,
				VGfloat * values)=0;


  virtual void vgDrawPath(VGPath path, VGbitfield paintModes)=0;
  virtual void vgDestroyPath(VGPath path)=0;
  virtual VGUErrorCode vguEllipse(VGPath path,
				  VGfloat cx, VGfloat cy,
				  VGfloat width, VGfloat height)=0;

  virtual VGPath vgCreatePath(VGint pathFormat,
			      VGPathDatatype datatype,
			      VGfloat scale, VGfloat bias,
			      VGint segmentCapacityHint,
			      VGint coordCapacityHint,
			      VGbitfield capabilities)=0;

};
#endif
