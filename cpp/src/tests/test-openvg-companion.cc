#include <boost/filesystem.hpp>

#include <gmock/gmock.h>

// include mocks
#include "tests/test-openvg-adapter.hh"

// include objects under test
#include "assets/assets.hh"
#include "gfx/openvg-companion.hh"


using ::testing::_;
using ::testing::Eq;
using ::testing::IsNull;
using ::testing::An;
using ::testing::Return;
using ::testing::TypedEq;

using namespace std;

namespace fs = boost::filesystem;


TEST(OpenVGCompanionTests, TestImageDrawing) {
  TestOpenvgAdaptper ovg_adapter;
  
  // we don't care about the font-setup
  EXPECT_CALL(ovg_adapter, 
	      vgAppendPathData(
		  An<VGPath>(),
		  An<VGint>(),
		  An<const VGubyte*>(),
		  An<const void*>()
	      )
  ).WillRepeatedly(Return());

  EXPECT_CALL(ovg_adapter,
	      vgCreatePath(
		  An<VGint>(),
		  An<VGPathDatatype>(),
		  An<VGfloat>(),
		  An<VGfloat>(),
		  An<VGint>(),
		  An<VGint>(),
		  An<VGbitfield>()
	      )
  ).WillRepeatedly(Return(1));


  // the real calls we're interested in

  EXPECT_CALL(ovg_adapter, vgCreateImage(
		  TypedEq<VGImageFormat>(VG_sRGBA_8888), 
		  TypedEq<VGint>(264), 
		  TypedEq<VGint>(258), 
		  TypedEq<VGbitfield>(VG_IMAGE_QUALITY_BETTER))
  ).WillOnce(Return(1));

  EXPECT_CALL(ovg_adapter,
	      vgImageSubData(
		  Eq(1),
		  An<const void *>(),
		  Eq(264*4),
		  Eq(VG_sRGBA_8888),
		  TypedEq<VGint>(0), 
		  TypedEq<VGint>(0), 
		  TypedEq<VGint>(264), 
		  TypedEq<VGint>(258)
	      )
  ).Times(1);


  EXPECT_CALL(ovg_adapter,
	      vgSetPixels(
		  Eq(100), // x
		  Eq(200), // y
		  Eq(1), // img_handle
		  Eq(0), // src_x
		  Eq(0), // src_y
		  Eq(264), // width
		  Eq(258) // height
	      )
  ).Times(1);


  EXPECT_CALL(ovg_adapter,
   	      vgDestroyImage(
  		  Eq(1)
 	      )
  ).Times(1);
  
  // TEST
  fs::path image_path("amiga-ball.png");
  ASSERT_TRUE(fs::exists(image_path));
  AssetManager am(&ovg_adapter);
  OpenVGCompanion vgc(ovg_adapter);
  const PNGImageData &image_data = am.image(image_path);
  
  vgc.drawImage(image_data, 100, 200);
  
}

