#include <iostream>
#include "posix-adapter-impl.hh"
#include "terminal.hh"

using namespace std;

int main(int argc, char **argv) {
  PosixAdapterImpl posix_adapter;
  Terminal terminal(posix_adapter);
  terminal.install_signal_handler();
  while(true) {
    int c = terminal.read_character();
    if(c != -1) {
      cout << "scancode: " << c << endl;
      if(c == 1) {
	break;
      }
    }
  }
}
