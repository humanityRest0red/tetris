/***
 * @file tetris.c
 * @brief BRICK_GAME TETRIS source
 */

#include "tetris.h"

#include <stdio.h>
#include <stdlib.h>

ActionT fsmTable[][8] = {
    {startGame, NULL, terminateGame, NULL, NULL, NULL, NULL, NULL},  // start
    {startGame, terminateGame, terminateGame, terminateGame, terminateGame,
     terminateGame, terminateGame, terminateGame},  // gameover
    {spawn, spawn, terminateGame, spawn, spawn, spawn, spawn, spawn},  // spawn
    {NULL, pauseGame, terminateGame, moveLeft, moveRight, NULL, moveDown,
     rotate},                                                  // moving
    {shift, shift, terminateGame, shift, shift, shift, shift,  // shifting
     shift},
    {attach, attach, terminateGame, attach, attach, attach, attach,
     attach},  // attaching
};

GameInfo_t updateCurrentState(void) {
  GameParams_t *prms = updateParams(NULL);
  if (prms->state != GAMEOVER) prms->state = SHIFTING;

  return prms->info;
}

void userInput(UserAction_t action, bool hold) {
  GameParams_t *prms = updateParams(NULL);
  ActionT func = fsmTable[prms->state][action];
  if (func) {
    if (prms->info.pause) {
      if (func == pauseGame) {
        func(prms);
      }
    } else {
      if (func == moveDown && hold) {
        for (int i = 0; i < HEIGHT; ++i) {
          moveDown(prms);
        }
        prms->state = SHIFTING;
      } else {
        func(prms);
      }
    }
  }
}

GameParams_t *updateParams(GameParams_t *params) {
  static GameParams_t *data;
  if (params != NULL) data = params;
  return data;
}

void initializeParams(GameParams_t *prms) {
  prms->info.field = newArray2D(HEIGHT, WIDTH);
  prms->info.next = newArray2D(TETROMINO_HEIGHT, TETROMINO_WIDTH);
  prms->info.score = 0;
  prms->info.high_score = readHighScore();
  prms->info.level = 1;
  prms->info.speed = 1;
  prms->info.pause = false;

  prms->tetromino.cells[0].y = 1;
  prms->tetromino.cells[0].x = 3;
  prms->tetromino.cells[1].y = 1;
  prms->tetromino.cells[1].x = 4;
  prms->tetromino.cells[2].y = 1;
  prms->tetromino.cells[2].x = 5;
  prms->tetromino.cells[3].y = 1;
  prms->tetromino.cells[3].x = 6;

  prms->tetromino.name_next = 0;

  prms->state = START;
}

void spawn(GameParams_t *prms) {
  const Tetromino_t tetrominoes[] = {
      [I].cells = {{0, 1}, {1, 1}, {2, 1}, {3, 1}},   // ....
      [J].cells = {{0, 0}, {0, 1}, {1, 1}, {2, 1}},   // :..
      [L].cells = {{2, 0}, {0, 1}, {1, 1}, {2, 1}},   // ..:
      [O].cells = {{1, 0}, {2, 0}, {1, 1}, {2, 1}},   //  ::
      [S].cells = {{1, 0}, {2, 0}, {0, 1}, {1, 1}},   // .:'
      [T].cells = {{1, 0}, {0, 1}, {1, 1}, {2, 1}},   // .:.
      [Z].cells = {{0, 0}, {1, 0}, {1, 1}, {2, 1}}};  // ':.

  int name_now = prms->tetromino.name_next;
  prms->tetromino.name = name_now;
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    prms->tetromino.cells[i].x =
        WIDTH / 2 - 2 + tetrominoes[name_now].cells[i].x;
    prms->tetromino.cells[i].y = tetrominoes[name_now].cells[i].y;
  }

  prms->tetromino.name_next = rand() % 7;
  Tetromino_t next = tetrominoes[prms->tetromino.name_next];

  for (int i = 0; i < TETROMINO_HEIGHT; ++i) {
    for (int j = 0; j < TETROMINO_WIDTH; ++j) {
      prms->info.next[i][j] = 0;
    }
  }
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    prms->info.next[next.cells[i].y][next.cells[i].x] = 1;
  }

  prms->state = MOVING;
}

void startGame(GameParams_t *prms) { prms->state = SPAWN; }

void pauseGame(GameParams_t *prms) { prms->info.pause = 1 - prms->info.pause; }

void terminateGame(GameParams_t *prms) {
  deleteArray2D(prms->info.field, HEIGHT);
  deleteArray2D(prms->info.next, TETROMINO_HEIGHT);
  prms->state = GAMEOVER;
}

void shift(GameParams_t *prms) {
  if (isAttach(prms)) {
    prms->state = ATTACHING;
  } else {
    moveDown(prms);
    prms->state = MOVING;
  }
}

void moveDown(GameParams_t *prms) {
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) prms->tetromino.cells[i].y++;
  if (isCollide(prms)) {
    for (int i = 0; i < CELLS_IN_TETROMINO; ++i) prms->tetromino.cells[i].y--;
  }
}

void moveRight(GameParams_t *prms) {
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) prms->tetromino.cells[i].x++;
  if (isCollide(prms)) {
    for (int i = 0; i < CELLS_IN_TETROMINO; ++i) prms->tetromino.cells[i].x--;
  }
}

void moveLeft(GameParams_t *prms) {
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) prms->tetromino.cells[i].x--;
  if (isCollide(prms)) {
    for (int i = 0; i < CELLS_IN_TETROMINO; ++i) prms->tetromino.cells[i].x++;
  }
}

void rotate(GameParams_t *prms) {
  // if figure is square
  if (prms->tetromino.name == O) {
    return;
  }

  Cell_t center;
  if (prms->tetromino.name == I || prms->tetromino.name == Z)
    center = prms->tetromino.cells[1];
  else if (prms->tetromino.name == J || prms->tetromino.name == L)
    center = prms->tetromino.cells[2];
  else
    center = prms->tetromino.cells[0];

  Tetromino_t temp;
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    temp.cells[i].x = prms->tetromino.cells[i].x;
    temp.cells[i].y = prms->tetromino.cells[i].y;
  }

  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    prms->tetromino.cells[i].x = center.x + center.y - temp.cells[i].y;
    prms->tetromino.cells[i].y = center.y - center.x + temp.cells[i].x;
  }

  if (isCollide(prms))
    for (int i = 0; i < CELLS_IN_TETROMINO; ++i)
      prms->tetromino.cells[i] = temp.cells[i];
}

bool isCollide(GameParams_t *prms) {
  bool check = false;
  for (int i = 0; i < CELLS_IN_TETROMINO && !check; ++i) {
    if (prms->tetromino.cells[i].x < 0 || prms->tetromino.cells[i].x >= WIDTH ||
        prms->tetromino.cells[i].y < 0 ||
        prms->tetromino.cells[i].y >= HEIGHT ||
        prms->info
            .field[prms->tetromino.cells[i].y][prms->tetromino.cells[i].x])
      check = true;
  }

  return check;
}

bool isAttach(GameParams_t *prms) {
  bool check = false;
  for (int i = 0; i < CELLS_IN_TETROMINO && !check; ++i)
    if (prms->tetromino.cells[i].y == HEIGHT - 1 ||
        prms->info
            .field[prms->tetromino.cells[i].y + 1][prms->tetromino.cells[i].x])
      check = true;

  return check;
}

void attach(GameParams_t *prms) {
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    prms->info.field[prms->tetromino.cells[i].y][prms->tetromino.cells[i].x] =
        true;
  }

  const int scores[5] = {0, 100, 300, 700, 1500};
  prms->info.score += scores[linesDisappeared(prms->info.field)];
  if (prms->info.score > prms->info.high_score) {
    writeHighScore(prms->info.score);
    prms->info.high_score = prms->info.score;
  }
  while (prms->info.score >= prms->info.level * 600 && prms->info.level < 10) {
    ++prms->info.level;
  }
  prms->info.speed = prms->info.level;
  prms->state = canSpawn(prms) ? SPAWN : GAMEOVER;
}

int linesDisappeared(int **map) {
  int result = 0;
  for (int i = 0; i < HEIGHT; ++i) {
    bool check = true;
    for (int j = 0; j < WIDTH && check; ++j) {
      if (!map[i][j]) check = false;
    }
    if (check) {
      ++result;
      for (int ind = i; ind >= 1; --ind) {
        for (int j = 0; j < WIDTH; ++j) {
          map[ind][j] = map[ind - 1][j];
        }
      }
    }
  }
  return result;
}

bool canSpawn(GameParams_t *prms) {
  bool check = true;
  for (int i = 0; i < TETROMINO_HEIGHT && check; ++i) {
    for (int j = 0; j < TETROMINO_WIDTH && check; ++j) {
      if (prms->info.next[i][j] && prms->info.field[i][j + (WIDTH / 2 - 2)])
        check = false;
    }
  }

  return check;
}

int **newArray2D(int n, int m) {
  int **res = calloc(sizeof(int *), n);
  if (res) {
    for (int i = 0; i < n; ++i) {
      res[i] = calloc(sizeof(int), m);
    }
  }

  return res;
}

void deleteArray2D(int **this, int n) {
  if (this) {
    for (int i = 0; i < n; ++i) free(this[i]);
  }
  free(this);
}

int readHighScore(void) {
  int high_score;
  FILE *f = fopen(HIGH_SCORE_FILE, "r");
  fscanf(f, "%d", &high_score);
  fclose(f);

  return high_score;
}

void writeHighScore(int high_score) {
  FILE *f = fopen(HIGH_SCORE_FILE, "w");
  fprintf(f, "%d", high_score);
  fclose(f);
}