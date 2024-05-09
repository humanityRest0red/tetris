#include "suitecases.h"

START_TEST(canSpawn_true_1) {
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  prms->info.field[1][2] = true;

  for (int j = 0; j < CELLS_IN_TETROMINO; ++j) {
    prms->info.next[1][j] = true;
  }
  ck_assert_int_eq(canSpawn(prms), true);

  terminateGame(prms);
}
END_TEST

START_TEST(canSpawn_false_1) {
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  prms->info.field[1][3] = true;

  for (int j = 0; j < CELLS_IN_TETROMINO; ++j) {
    prms->info.next[1][j] = true;
  }
  ck_assert_int_eq(canSpawn(prms), false);

  terminateGame(prms);
}
END_TEST

Suite* suite_canSpawn(void) {
  Suite* s = suite_create("canSpawn");
  TCase* tc = tcase_create("canSpawn");

  tcase_add_test(tc, canSpawn_true_1);
  tcase_add_test(tc, canSpawn_false_1);

  suite_add_tcase(s, tc);
  return s;
}
