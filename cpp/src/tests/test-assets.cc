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

class AssetTests : public ::testing::Test {
public:
  TestOpenvgAdaptper* ovg_adapter;
  virtual void SetUp() {
    ovg_adapter = new TestOpenvgAdaptper();
    
    // we don't care about the font-setup
    EXPECT_CALL(*ovg_adapter, 
		vgAppendPathData(
		    An<VGPath>(),
		    An<VGint>(),
		    An<const VGubyte*>(),
		    An<const void*>()
		)
    ).WillRepeatedly(Return());

    EXPECT_CALL(*ovg_adapter,
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
  }
  
  virtual void TearDown() {
    delete ovg_adapter;
  }
};

TEST_F(AssetTests, TestImageManagement) {
  fs::path image_path("resources/tests/amiga-ball.png");
  ASSERT_TRUE(fs::exists(image_path));
  {
    AssetManager am(*ovg_adapter);
    const PNGImageData& img = am.image(image_path);
    ASSERT_EQ(300, img.width());
    ASSERT_EQ(300, img.height());
  }
}


TEST_F(AssetTests, TestRelativeResourcePaths) {
  fs::path resources_path("resources");
  fs::path image_path("tests/amiga-ball.png");
  {
    AssetManager am(*ovg_adapter, resources_path);
    const PNGImageData& img = am.image(image_path);
    ASSERT_EQ(300, img.width());
    ASSERT_EQ(300, img.height());
  }
}



