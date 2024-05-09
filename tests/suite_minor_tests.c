#include "suitecases.h"

START_TEST(test_initializePrms) {
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  ck_assert_int_eq(prms->info.pause, false);
  ck_assert_ptr_nonnull(prms->info.field);
  ck_assert_ptr_nonnull(prms->info.next);
  ck_assert_int_eq(prms->info.level, 1);
  ck_assert_int_eq(prms->info.speed, 1);
  ck_assert_int_eq(prms->info.score, 0);
  ck_assert_int_eq(prms->state, START);

  terminateGame(prms);
}
END_TEST

START_TEST(test_startGame) {
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  startGame(prms);
  ck_assert_int_eq(prms->state, SPAWN);

  terminateGame(prms);
}
END_TEST

START_TEST(test_pauseGame) {
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  ck_assert_int_eq(prms->info.pause, false);
  pauseGame(prms);
  ck_assert_int_eq(prms->info.pause, true);
  pauseGame(prms);
  ck_assert_int_eq(prms->info.pause, false);

  terminateGame(prms);
}
END_TEST

START_TEST(test_spawn) {
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  spawn(prms);
  ck_assert_int_eq(prms->state, MOVING);

  terminateGame(prms);
}
END_TEST

START_TEST(test_write_high_score) {
  GameParams_t params;
  GameParams_t* prms = &params;
  initializeParams(prms);

  int old_high_score = readHighScore();
  prms->info.score = old_high_score + 100;
  attach(prms);
  ck_assert_int_eq(prms->info.score, readHighScore());
  writeHighScore(old_high_score);

  terminateGame(prms);
}
END_TEST

Suite* suite_minor_tests(void) {
  Suite* s = suite_create("minor_tests");
  TCase* tc = tcase_create("minor_tests");

  tcase_add_test(tc, test_initializePrms);
  tcase_add_test(tc, test_startGame);
  tcase_add_test(tc, test_pauseGame);
  tcase_add_test(tc, test_spawn);
  tcase_add_test(tc, test_write_high_score);

  suite_add_tcase(s, tc);
  return s;
}
