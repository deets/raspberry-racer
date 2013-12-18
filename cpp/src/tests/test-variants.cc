#include <gmock/gmock.h>
#include <boost/variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include "events/events.hh"

using ::testing::_;
using ::testing::Eq;
using ::testing::IsNull;
using ::testing::An;
using ::testing::Return;
using ::testing::TypedEq;
using ::testing::NiceMock;

using namespace std;
using namespace rracer;


typedef boost::variant<int, string> TheVariant;

class TheVariantVisitor : public boost::static_visitor<bool> {
  public:
  bool found_int;

  TheVariantVisitor() : found_int(false) {}

  virtual bool operator()(int i) {
    found_int = true;
    return true;
  }

  template<typename T> bool operator()(T ignored) {
    return true;
  }
};



class DerivedVariantVisitor : public TheVariantVisitor {
public:
  bool egal;
  virtual bool operator()(int i) {
    egal = true;
    return true;
  }

  template<typename T> bool operator()(T ignored) {
    return true;
  }

};

class VariantTests : public ::testing::Test {
public:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

};  


TEST_F(VariantTests, TestVariantVisiting) {
  TheVariantVisitor visitor;
  TheVariant v = 10;
  boost::apply_visitor(visitor, v);
  boost::apply_visitor(visitor, v);
  ASSERT_TRUE(visitor.found_int);
}



TEST_F(VariantTests, TestVariantVisitingWithSubclass) {
  DerivedVariantVisitor visitor;
  visitor.egal = false;
  TheVariant v = 10;
  boost::apply_visitor(visitor, v);
  ASSERT_TRUE(visitor.egal);
  ASSERT_FALSE(visitor.found_int);
}
