#include "posix-adapter-impl.hh"
#include "terminal.hh"


int main(int argc, char **argv) {
  PosixAdapterImpl posix_adapter;
  Terminal terminal(posix_adapter);
  terminal.install_signal_handler();

}
