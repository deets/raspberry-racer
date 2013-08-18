#ifndef RPI_WINDOW_ADAPTER_HH
#define RPI_WINDOW_ADAPTER_HH

#include <utility>
#include "GLES/gl.h"
#include "EGL/egl.h"
#include "gfx/window-adapter.hh"
#include "gfx/openvg-adapter.hh"
#include "common/non-copyable.hh"
#include "eglstate.h"

using namespace std;



class RPiWindowAdapter : public WindowAdapter, public OpenVGAdapter, public NonCopyAble {

  uint32_t _width, _height;

  STATE_T _state;

public:
  RPiWindowAdapter();
  virtual ~RPiWindowAdapter();

  // WindowAdapter implementation
  virtual pair<int, int> window_dimensions();
  virtual void start();
  virtual void end();

  // OpenVGAdapter implementation
  virtual void vgSetf (VGParamType type, VGfloat value) const;
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
				VGint * values) const;

  virtual void vgSetParameterf(VGHandle object,
                                 VGint paramType,
			       VGfloat value) const;

  virtual void vgSetParameterfv(VGHandle object,
				VGint paramType,
				VGint count,
				VGfloat * values) const;


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


  virtual void vgDestroyImage(VGImage) const;
};

#endif
