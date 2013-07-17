#include <stdio.h>
#include "terminal.hh"


int main(int argc, char **argv) {
  Terminal t;
  t.install_signal_handler();
  for(;;) {
    int c = t.read_character();
    if( c == 1 ) {
      printf("quit\n");
      break;
    }
    if(c != -1) {
      printf("scancode: %i\n", c);
    }
  }
}
