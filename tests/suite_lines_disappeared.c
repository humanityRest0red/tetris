#include "suitecases.h"

START_TEST(linesDisappeared_1) {
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  for (int filled_lines = 0; filled_lines <= 4; ++filled_lines) {
    for (int i = 0; i < filled_lines; ++i) {
      for (int j = 0; j < WIDTH; ++j) {
        prms->info.field[HEIGHT - 1 - i][j] = true;
      }
    }
    ck_assert_int_eq(linesDisappeared(prms->info.field), filled_lines);
  }

  terminateGame(prms);
}
END_TEST

Suite* suite_linesDisappeared(void) {
  Suite* s = suite_create("linesDisappeared");
  TCase* tc = tcase_create("linesDisappeared");

  tcase_add_test(tc, linesDisappeared_1);

  suite_add_tcase(s, tc);
  return s;
}
