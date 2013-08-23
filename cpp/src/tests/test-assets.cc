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
  ).WillRepeatedly(Return((VGPath)1));

  fs::path image_path("amiga-ball.png");
  ASSERT_TRUE(fs::exists(image_path));
  
  {
    AssetManager am(&ovg_adapter);
    const PNGImageData& img = am.image(image_path);
    ASSERT_EQ(264, img.width());
    ASSERT_EQ(258, img.height());
  }

}

