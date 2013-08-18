#ifndef TEST_WINDOW_ADAPTER_HH
#define TEST_WINDOW_ADAPTER_HH

#include <gmock/gmock.h>

#include "gfx/window-adapter.hh"

using namespace std;

typedef pair<int, int> int_pair;

class TestWindowAdaptper : public WindowAdapter {

public:
  MOCK_METHOD0(window_dimensions, int_pair());
  MOCK_METHOD0(start, void());
  MOCK_METHOD0(end, void());
};
#endif
