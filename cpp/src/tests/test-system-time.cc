#include <time.h>
#include <vector>

#include <gmock/gmock.h>

#include "common/time.hh"

class TestTimer : public Timer {

public:
  std::vector<clock_t> timestamps;
  size_t index;
  TestTimer() : Timer(), index(0) {}

  clock_t clock() {
    return timestamps[index++];
  }

};

TEST(SystemTimeTests, TestTick) {
  TestTimer t;
  clock_t ts = CLOCKS_PER_SEC * 1;
  t.timestamps.push_back(ts);
  ts += CLOCKS_PER_SEC * 1;
  t.timestamps.push_back(ts);
  ASSERT_FLOAT_EQ(1.0, t.tick());
  ASSERT_FLOAT_EQ(2.0, t.tick());
}


TEST(SystemTimeTests, TestElapsed) {
  TestTimer t;
  t.timestamps.push_back(CLOCKS_PER_SEC);
  t.timestamps.push_back(CLOCKS_PER_SEC * 3);
  ASSERT_FLOAT_EQ(.0, t.elapsed());
  ASSERT_FLOAT_EQ(2.0, t.elapsed());
}
