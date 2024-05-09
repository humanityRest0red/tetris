#include "suitecases.h"

START_TEST(isCollide_true_1) {
  // with field
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  prms->info.field[15][0] = 1;

  prms->tetromino.cells[0].y = 15;
  prms->tetromino.cells[0].x = 0;
  ck_assert_int_eq(isCollide(prms), true);

  terminateGame(prms);
}
END_TEST

Suite* suite_isCollide(void) {
  Suite* s = suite_create("isCollide");
  TCase* tc = tcase_create("isCollide");

  tcase_add_test(tc, isCollide_true_1);
  //   tcase_add_test(tc, isCollide_true_2);

  suite_add_tcase(s, tc);
  return s;
}
