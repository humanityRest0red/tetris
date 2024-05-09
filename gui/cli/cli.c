/***
 * @file cli.c
 * @brief CLI GUI source
 */

#include "cli.h"

void myWinInit() {
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, 1);
  halfdelay(0);
  start_color();
  init_color(COLOR_ORANGE, 1000, 670, 0);
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  init_pair(2, COLOR_BLUE, COLOR_BLACK);
  init_pair(3, COLOR_ORANGE, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_GREEN, COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(7, COLOR_RED, COLOR_BLACK);
  setlocale(LC_ALL, "");
  srand(time(NULL));
  timeout(1);
}

void gameLoop() {
  GameParams_t prms;
  initializeParams(&prms);
  updateParams(&prms);

  int **field_colors = newArray2D(HEIGHT, WIDTH);
  double counter = 0.;
  int click_old = clock();
  int spawn_time = 0;
  bool loop = true;
  while (loop) {
    if (prms.state == SPAWN) {
      spawn_time = clock();
    }
    UserAction_t action = getAction(getch());
    bool hold = false;
    if (action == Down) {
      int click_new = clock();
      if (click_new - click_old <= 3000 && click_new - spawn_time > 30000) {
        hold = true;
      }
      click_old = click_new;
    }
    userInput(action, hold);

    if (prms.info.pause) {
      drawPause();
      continue;
    }

    if (counter >= 1000.0) {
      if (prms.state == MOVING) {
        updateCurrentState();
      }
      counter = 0.;
    }
    counter += prms.info.speed;

    if (prms.state == START) {
      drawGui();
    } else if (prms.state == GAMEOVER) {
      deleteArray2D(field_colors, HEIGHT);
      loop = false;
    } else if (prms.state != SPAWN) {
      if (prms.state == ATTACHING) {
        for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
          field_colors[prms.tetromino.cells[i].y][prms.tetromino.cells[i].x] =
              prms.tetromino.name + 1;
        }
        linesDisappeared(field_colors);
      }
      drawInfo(&prms.info, prms.tetromino.name_next);
      drawField(prms.info.field, field_colors);
      drawTetromino(&prms.tetromino);
    }
  }
}

UserAction_t getAction(int user_input) {
  UserAction_t action = Up;

  if (user_input == ENTER_KEY)
    action = Start;
  else if (user_input == PAUSE_KEY)
    action = Pause;
  else if (user_input == ESCAPE_KEY)
    action = Terminate;
  else if (user_input == KEY_LEFT)
    action = Left;
  else if (user_input == KEY_RIGHT)
    action = Right;
  else if (user_input == KEY_DOWN)
    action = Down;
  else if (user_input == ACTION_KEY)
    action = Action;

  return action;
}

void drawGui(void) {
  drawRectangle(0, BOARD_N + 1, 0, BOARD_M + 1);
  drawRectangle(0, BOARD_N + 1, BOARD_M + 1, BOARD_M + HUD_WIDTH + 1);

  drawRectangle(Y_NEXT - 1, Y_NEXT + 5, BOARD_M + 3, BOARD_M + HUD_WIDTH);
  drawRectangle(Y_SCORE - 1, Y_SCORE + 3, BOARD_M + 3, BOARD_M + HUD_WIDTH);
  drawRectangle(Y_HIGH_SCORE - 1, Y_HIGH_SCORE + 3, BOARD_M + 3,
                BOARD_M + HUD_WIDTH);
  drawRectangle(Y_LEVEL - 1, Y_LEVEL + 1, BOARD_M + 3, BOARD_M + HUD_WIDTH);

  MVPRINTW(Y_NEXT, BOARD_M + 8, "Next");
  MVPRINTW(Y_SCORE, BOARD_M + 7, "Score");
  MVPRINTW(Y_HIGH_SCORE, BOARD_M + 5, "High score");
  MVPRINTW(Y_LEVEL, BOARD_M + 5, "Level");

  MVPRINTW(BOARD_N / 2, (BOARD_M - INTRO_MESSAGE_LEN) / 2 + 1, INTRO_MESSAGE);

  mvprintw(2, BOARD_N * 2 + 2, "ENTER - start game");
  mvprintw(3, BOARD_N * 2 + 2, "Use the arrow keys to move down, right, left.");
  mvprintw(4, BOARD_N * 2 + 2, "Hold key down to make the figure fall.");
  mvprintw(5, BOARD_N * 2 + 2, "SPACE - rotate figure.");
  mvprintw(6, BOARD_N * 2 + 2, "P - pause game.");
  mvprintw(7, BOARD_N * 2 + 2, "ESC - quit game.");
}

void drawRectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) MVADDCH(top_y, i, ACS_HLINE);
  MVADDCH(top_y, i, ACS_URCORNER);

  for (i = top_y + 1; i < bottom_y; i++) {
    MVADDCH(i, left_x, ACS_VLINE);
    MVADDCH(i, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) MVADDCH(bottom_y, i, ACS_HLINE);
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void drawInfo(GameInfo_t *info, int name_next) {
  int color = name_next + 1;
  for (int i = 0; i < TETROMINO_HEIGHT; ++i) {
    for (int j = 0; j < TETROMINO_WIDTH; ++j) {
      attron(COLOR_PAIR(color));
      mvprintw(Y_NEXT + 4 + (i), BOARDS_BEGIN + (BOARD_M + 5 + j * 2 + 1),
               info->next[i][j] ? "[]" : "  ");
      attroff(COLOR_PAIR(color));
    }
  }
  MVPRINTW(Y_SCORE + 2, BOARD_M + 10, "%d", info->score);
  MVPRINTW(Y_HIGH_SCORE + 2, BOARD_M + 10, "%d", info->high_score);
  MVPRINTW(Y_LEVEL, BOARD_M + 12, "%d", info->level);
}

void drawTetromino(Tetromino_t *tetromino) {
  if (tetromino->cells[0].y == 0 && tetromino->cells[0].x == 0) return;

  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    drawCell(tetromino->cells[i].y, tetromino->cells[i].x, true,
             tetromino->name + 1);
  }
}

void drawField(int **field, int **field_colors) {
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      drawCell(i, j, field[i][j], field_colors[i][j]);
    }
  }
}

void drawCell(int i, int j, bool isCell, int color) {
  attron(COLOR_PAIR(color));
  mvprintw(1 + BOARDS_BEGIN + (i), BOARDS_BEGIN + (j * 2 + 1),
           isCell ? "[]" : "  ");
  attroff(COLOR_PAIR(color));
}

void drawPause() { mvprintw(1 + BOARD_M / 2, BOARD_N / 2, "PAUSE"); }

void drawGameOver() { 
  attron(COLOR_PAIR(COLOR_RED));
  mvprintw(9, BOARD_N * 2 + 2, "GAME OVER");
  attroff(COLOR_PAIR(COLOR_RED));
}

