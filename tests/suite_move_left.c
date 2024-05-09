#include "suitecases.h"

START_TEST(moveLeft_false_1) {
  // collide with field
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  prms->info.field[15][0] = true;
  Tetromino_t etalon;
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    prms->tetromino.cells[i].y = etalon.cells[i].y = 15;
    prms->tetromino.cells[i].x = etalon.cells[i].x = i + 1;
  }
  moveLeft(prms);

  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    ck_assert_int_eq(prms->tetromino.cells[i].y, etalon.cells[i].y);
    ck_assert_int_eq(prms->tetromino.cells[i].x, etalon.cells[i].x);
  }

  terminateGame(prms);
}
END_TEST

START_TEST(moveLeft_false_2) {
  // collide with wall
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  Tetromino_t etalon;
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    prms->tetromino.cells[i].y = etalon.cells[i].y = 15;
    prms->tetromino.cells[i].x = etalon.cells[i].x = i;
  }

  moveLeft(prms);

  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    ck_assert_int_eq(prms->tetromino.cells[i].y, etalon.cells[i].y);
    ck_assert_int_eq(prms->tetromino.cells[i].x, etalon.cells[i].x);
  }
  terminateGame(prms);
}
END_TEST

START_TEST(moveLeft_true_1) {
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  prms->info.field[15][0] = true;
  Tetromino_t etalon;
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    prms->tetromino.cells[i].y = etalon.cells[i].y = 14;  // no 15
    prms->tetromino.cells[i].x = etalon.cells[i].x = i + 1;
  }
  moveLeft(prms);

  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    ck_assert_int_eq(prms->tetromino.cells[i].y, etalon.cells[i].y);
    ck_assert_int_eq(prms->tetromino.cells[i].x, etalon.cells[i].x - 1);
  }

  terminateGame(prms);
}
END_TEST

START_TEST(moveLeft_true_2) {
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  Tetromino_t etalon;
  for (int i = 0; i < 3; ++i) {
    prms->tetromino.cells[i].y = etalon.cells[i].y = 15;
    prms->tetromino.cells[i].x = etalon.cells[i].x = i + 1;
  }
  prms->tetromino.cells[3].y = etalon.cells[3].y = 14;
  prms->tetromino.cells[3].x = etalon.cells[3].x = 2;

  moveLeft(prms);

  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    ck_assert_int_eq(prms->tetromino.cells[i].y, etalon.cells[i].y);
    ck_assert_int_eq(prms->tetromino.cells[i].x, etalon.cells[i].x - 1);
  }
  terminateGame(prms);
}
END_TEST

Suite* suite_moveLeft(void) {
  Suite* s = suite_create("moveLeft");
  TCase* tc = tcase_create("moveLeft");

  tcase_add_test(tc, moveLeft_false_1);
  tcase_add_test(tc, moveLeft_false_2);

  tcase_add_test(tc, moveLeft_true_1);
  tcase_add_test(tc, moveLeft_true_2);

  suite_add_tcase(s, tc);
  return s;
}
