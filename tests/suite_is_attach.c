#include "suitecases.h"

START_TEST(isAttach_true_1) {
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  prms->info.field[15][0] = 1;

  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    prms->tetromino.cells[i].y = 14;
    prms->tetromino.cells[i].x = i;
  }
  ck_assert_int_eq(isAttach(prms), true);

  terminateGame(prms);
}
END_TEST

START_TEST(isAttach_false_1) {
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  prms->info.field[15][0] = 1;
  for (int i = 0; i < CELLS_IN_TETROMINO; ++i) {
    prms->tetromino.cells[i].y = 14;
    prms->tetromino.cells[i].x = i + 1;
  }
  ck_assert_int_eq(isAttach(prms), false);

  terminateGame(prms);
}
END_TEST

Suite* suite_isAttach(void) {
  Suite* s = suite_create("isAttach");
  TCase* tc = tcase_create("isAttach");

  tcase_add_test(tc, isAttach_true_1);
  tcase_add_test(tc, isAttach_false_1);

  suite_add_tcase(s, tc);
  return s;
}
