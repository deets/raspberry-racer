#ifndef RPI_WINDOW_ADAPTER_HH
#define RPI_WINDOW_ADAPTER_HH

#include <utility>
#include "GLES/gl.h"
#include "EGL/egl.h"
#include "window-adapter.hh"
#include "openvg-adapter.hh"
#include "non-copyable.hh"
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
  virtual void vgSetf (VGParamType type, VGfloat value);
  virtual void vgClear(VGint x, VGint y, VGint width, VGint height);
  virtual void vgLoadIdentity();
  virtual VGPaint vgCreatePaint();
  virtual void vgDestroyPaint(VGPaint paint);
  virtual void vgSetPaint(VGPaint paint, VGbitfield paintModes);
  virtual void vgSetParameteri(VGHandle object,
			       VGint paramType,
			       VGint value);

  virtual void vgSetParameteriv(VGHandle object,
				VGint paramType,
				VGint count,
				VGint * values);

  virtual void vgSetParameterf(VGHandle object,
                                 VGint paramType,
			       VGfloat value);

  virtual void vgSetParameterfv(VGHandle object,
				VGint paramType,
				VGint count,
				VGfloat * values);


  virtual void vgDrawPath(VGPath path, VGbitfield paintModes);
  virtual void vgDestroyPath(VGPath path);
  virtual VGUErrorCode vguEllipse(VGPath path,
				  VGfloat cx, VGfloat cy,
				  VGfloat width, VGfloat height);

  virtual VGPath vgCreatePath(VGint pathFormat,
			      VGPathDatatype datatype,
			      VGfloat scale, VGfloat bias,
			      VGint segmentCapacityHint,
			      VGint coordCapacityHint,
			      VGbitfield capabilities);


};

#endif
