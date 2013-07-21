#ifndef RPI_ADAPTER_HH
#define RPI_ADAPTER_HH

#include <stdint.h>
#include "posix-adapter.hh"

class RPiAdapter : public PosixAdapter {

public:
  virtual void bcm_host_init()=0;
  virtual void bcm_host_deinit()=0;
  virtual int32_t graphics_get_display_size(
      const uint16_t display_number,
      uint32_t *width,
      uint32_t *height)=0;
};

#endif
