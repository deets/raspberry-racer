#ifndef TIME_HH
#define TIME_HH

#include <time.h>

class Timer {

  bool _elapsed;
  double _then;
public:

  Timer() : _elapsed(false) {}

  double tick() {
    return ((double)this->clock()) / CLOCKS_PER_SEC;
  }


  double elapsed() {
    if(!_elapsed) {
      _then = this->tick();
      _elapsed = true;
      return .0;
    }
    double now = this->tick();
    double elapsed = now - _then;
    _then = now;
    return elapsed;
  }


  virtual clock_t clock() {
    return ::clock();
  }

};
#endif
