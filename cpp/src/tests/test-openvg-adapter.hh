#ifndef TEST_OPENVG_ADAPTER_HH
#define TEST_OPENVG_ADAPTER_HH

#include <gmock/gmock.h>

#include "openvg-adapter.hh"


class TestOpenvgAdaptper : public OpenVGAdapter {

  MOCK_METHOD2(vgSetf, void(VGParamType, VGfloat));
  MOCK_METHOD4(vgClear, void(VGint, VGint, VGint, VGint));
  MOCK_METHOD0(vgLoadIdentity, void());
  MOCK_METHOD0(vgCreatePaint, VGPaint());
  MOCK_METHOD2(vgSetPaint, void(VGPaint, VGbitfield));
  MOCK_METHOD3(vgSetParameteri, void(VGHandle, VGint, VGint));
  MOCK_METHOD3(vgSetParameterf, void(VGHandle, VGint, VGfloat));
  MOCK_METHOD4(vgSetParameteriv, void(VGHandle, VGint, VGint, VGint*));
  MOCK_METHOD4(vgSetParameterfv, void(VGHandle, VGint, VGint, VGfloat*));
  MOCK_METHOD2(vgDrawPath, void(VGPath, VGbitfield));
  MOCK_METHOD1(vgDestroyPath, void(VGPath));
  MOCK_METHOD1(vgDestroyPaint, void(VGPaint));
  MOCK_METHOD5(vguEllipse, VGUErrorCode(VGPath, VGfloat, VGfloat, VGfloat, VGfloat));
  MOCK_METHOD7(vgCreatePath, VGPath(VGint, VGPathDatatype, VGfloat, VGfloat, VGint, VGint, VGbitfield));
};
#endif
