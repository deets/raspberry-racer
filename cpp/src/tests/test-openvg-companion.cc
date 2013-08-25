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
  // TestOpenvgAdaptper ovg_adapter;

  // VGImage img_handle((void*)1245);
  // VGPath path_handle((void*)4444);
  // // we don't care about the font-setup
  // EXPECT_CALL(ovg_adapter, 
  // 	      vgAppendPathData(
  // 		  An<VGPath>(),
  // 		  An<VGint>(),
  // 		  An<const VGubyte*>(),
  // 		  An<const void*>()
  // 	      )
  // ).WillRepeatedly(Return());

  // EXPECT_CALL(ovg_adapter,
  // 	      vgCreatePath(
  // 		  An<VGint>(),
  // 		  An<VGPathDatatype>(),
  // 		  An<VGfloat>(),
  // 		  An<VGfloat>(),
  // 		  An<VGint>(),
  // 		  An<VGint>(),
  // 		  An<VGbitfield>()
  // 	      )
  // ).WillRepeatedly(Return(path_handle));


  // // the real calls we're interested in

  // EXPECT_CALL(ovg_adapter, vgCreateImage(
  // 		  TypedEq<VGImageFormat>(VG_sRGBA_8888), 
  // 		  TypedEq<VGint>(300), 
  // 		  TypedEq<VGint>(300), 
  // 		  TypedEq<VGbitfield>(VG_IMAGE_QUALITY_BETTER))
  // ).WillOnce(Return(img_handle));

  // EXPECT_CALL(ovg_adapter,
  // 	      vgImageSubData(
  // 		  Eq(img_handle),
  // 		  An<const void *>(),
  // 		  Eq(300*4),
  // 		  Eq(VG_sRGBA_8888),
  // 		  TypedEq<VGint>(0), 
  // 		  TypedEq<VGint>(0), 
  // 		  TypedEq<VGint>(300), 
  // 		  TypedEq<VGint>(300)
  // 	      )
  // ).Times(1);


  // EXPECT_CALL(ovg_adapter,
  // 	      vgSetPixels(
  // 		  Eq(100), // x
  // 		  Eq(200), // y
  // 		  Eq(img_handle), // img_handle
  // 		  Eq(0), // src_x
  // 		  Eq(0), // src_y
  // 		  Eq(300), // width
  // 		  Eq(300) // height
  // 	      )
  // ).Times(1);


  // EXPECT_CALL(ovg_adapter,
  //  	      vgDestroyImage(
  // 		  Eq(img_handle)
  // 	      )
  // ).Times(1);
  
  // // TEST
  // fs::path image_path("resources/tests/amiga-ball.png");
  // ASSERT_TRUE(fs::exists(image_path));
  // AssetManager am(ovg_adapter);
  // OpenVGCompanion vgc(ovg_adapter);
  // const PNGImageData &image_data = am.image(image_path);
  
  // vgc.drawImage(image_data);
  
}

