#ifndef TEST_OPENVG_ADAPTER_HH
#define TEST_OPENVG_ADAPTER_HH

#include <gmock/gmock.h>

#include "openvg-adapter.hh"


class TestOpenvgAdaptper : public OpenVGAdapter {

  MOCK_CONST_METHOD2(vgSetf, void(VGParamType, VGfloat));
  MOCK_CONST_METHOD4(vgClear, void(VGint, VGint, VGint, VGint));
  MOCK_CONST_METHOD0(vgLoadIdentity, void());
  MOCK_CONST_METHOD0(vgCreatePaint, VGPaint());
  MOCK_CONST_METHOD2(vgSetPaint, void(VGPaint, VGbitfield));
  MOCK_CONST_METHOD3(vgSetParameteri, void(VGHandle, VGint, VGint));
  MOCK_CONST_METHOD3(vgSetParameterf, void(VGHandle, VGint, VGfloat));
  MOCK_CONST_METHOD4(vgSetParameteriv, void(VGHandle, VGint, VGint, VGint*));
  MOCK_CONST_METHOD4(vgSetParameterfv, void(VGHandle, VGint, VGint, VGfloat*));
  MOCK_CONST_METHOD2(vgDrawPath, void(VGPath, VGbitfield));
  MOCK_CONST_METHOD1(vgDestroyPath, void(VGPath));
  MOCK_CONST_METHOD1(vgDestroyPaint, void(VGPaint));
  MOCK_CONST_METHOD5(vguEllipse, VGUErrorCode(VGPath, VGfloat, VGfloat, VGfloat, VGfloat));
  MOCK_CONST_METHOD7(vgCreatePath, VGPath(VGint, VGPathDatatype, VGfloat, VGfloat, VGint, VGint, VGbitfield));
  MOCK_CONST_METHOD1(vgGetMatrix, void(VGfloat*));
  MOCK_CONST_METHOD1(vgLoadMatrix, void(VGfloat*));
  MOCK_CONST_METHOD1(vgMultMatrix, void(VGfloat*));

  MOCK_CONST_METHOD4(vgAppendPathData, void(VGPath, VGint, const VGubyte *, const void*));


  MOCK_CONST_METHOD4(vgCreateImage, VGImage(VGImageFormat, VGint, VGint, VGbitfield));

  MOCK_CONST_METHOD8(vgImageSubData, void(VGImage, const void *, VGint, VGImageFormat,
					  VGint, VGint, VGint, VGint));

};
#endif
