#include <iostream>
#include <boost/filesystem.hpp>

#include <gmock/gmock.h>

// include mocks
#include "tests/test-openvg-adapter.hh"

// include objects under test
#include "assets/assets.hh"

using ::testing::_;
using ::testing::Eq;
using ::testing::IsNull;
using ::testing::An;
using ::testing::Return;
using ::testing::TypedEq;

using namespace std;

namespace fs = boost::filesystem;


TEST(AssetTests, TestImageManagement) {
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
  ).WillOnce(Return(1)).WillOnce(Return(1));

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
  ).Times(2);

  // EXPECT_CALL(ovg_adapter,
  // 	      vgDestroyImage(
  // 		  Eq(img_handle)
  // 	      )
  // ).Times(1);
  
  fs::path image_path("amiga-ball.png");
  ASSERT_TRUE(fs::exists(image_path));
  
  {
    AssetManager am(&ovg_adapter);
    ImageInfo img = am.image(image_path);
    ImageInfo img2 = am.image(image_path);
    ASSERT_EQ(img.image, img2.image);
  }
}

