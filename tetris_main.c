#include <unistd.h>

#include "gui/cli/cli.h"

int main(void) {
  myWinInit();
  gameLoop();
  drawGameOver();
  // sleep(5);

  endwin();

  return 0;
}