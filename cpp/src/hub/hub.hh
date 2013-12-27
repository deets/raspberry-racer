#ifndef HUB_HH
#define HUB_HH

#include <boost/filesystem.hpp>
#include "common/common.hh"

namespace fs = boost::filesystem;

namespace rracer {

  class Hub {
  public:
    virtual ~Hub() {}
    virtual void setup(OpenVGAdapter& vg, WindowAdapter& window_adapter, fs::path bundle_path)=0;
    virtual bool frame(Real elapsed, const GameEventVector& _events)=0;
  };

}; // end ns::rracer
#endif
