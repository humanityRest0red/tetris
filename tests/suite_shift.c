#include "suitecases.h"

START_TEST(shift_to_attach_1) {
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  prms->info.field[15][1] = true;
  Tetromino_t etalon;
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    prms->tetromino.cells[i].y = etalon.cells[i].y = 14;
    prms->tetromino.cells[i].x = etalon.cells[i].x = i;
  }
  shift(prms);

  ck_assert_int_eq(prms->state, ATTACHING);
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    ck_assert_int_eq(prms->tetromino.cells[i].y, etalon.cells[i].y);
    ck_assert_int_eq(prms->tetromino.cells[i].x, etalon.cells[i].x);
  }

  terminateGame(prms);
}
END_TEST

START_TEST(shift_to_attach_2) {
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  Tetromino_t etalon;
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    prms->tetromino.cells[i].y = etalon.cells[i].y = HEIGHT - 1;
    prms->tetromino.cells[i].x = etalon.cells[i].x = i;
  }

  shift(prms);

  ck_assert_int_eq(prms->state, ATTACHING);
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    ck_assert_int_eq(prms->tetromino.cells[i].y, etalon.cells[i].y);
    ck_assert_int_eq(prms->tetromino.cells[i].x, etalon.cells[i].x);
  }
  terminateGame(prms);
}
END_TEST

START_TEST(shift_to_move_1) {
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  prms->info.field[15][1] = true;
  Tetromino_t etalon;
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    prms->tetromino.cells[i].y = etalon.cells[i].y = 14;
    prms->tetromino.cells[i].x = etalon.cells[i].x = i + 2;  // no 1
  }
  shift(prms);

  ck_assert_int_eq(prms->state, MOVING);
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    ck_assert_int_eq(prms->tetromino.cells[i].y, etalon.cells[i].y + 1);
    ck_assert_int_eq(prms->tetromino.cells[i].x, etalon.cells[i].x);
  }

  terminateGame(prms);
}
END_TEST

START_TEST(shift_to_move_2) {
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  Tetromino_t etalon;
  for (int i = 0; i < 3; ++i) {
    prms->tetromino.cells[i].y = etalon.cells[i].y = HEIGHT - 2;
    prms->tetromino.cells[i].x = etalon.cells[i].x = WIDTH - 2 - i;
  }
  prms->tetromino.cells[3].y = etalon.cells[3].y = HEIGHT - 3;
  prms->tetromino.cells[3].x = etalon.cells[3].x = WIDTH - 3;

  shift(prms);

  ck_assert_int_eq(prms->state, MOVING);
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    ck_assert_int_eq(prms->tetromino.cells[i].y, etalon.cells[i].y + 1);
    ck_assert_int_eq(prms->tetromino.cells[i].x, etalon.cells[i].x);
  }
  terminateGame(prms);
}
END_TEST

Suite* suite_shift(void) {
  Suite* s = suite_create("shift");
  TCase* tc = tcase_create("shift");

  tcase_add_test(tc, shift_to_attach_1);
  tcase_add_test(tc, shift_to_attach_2);

  tcase_add_test(tc, shift_to_move_1);
  tcase_add_test(tc, shift_to_move_2);

  suite_add_tcase(s, tc);
  return s;
}
