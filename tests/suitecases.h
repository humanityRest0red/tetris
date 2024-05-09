#ifndef TESTS_SUITECASES_H_
#define TESTS_SUITECASES_H_

#include <check.h>

#include "../brick_game/tetris/tetris.h"

Suite *suite_moveDown(void);
Suite *suite_moveRight(void);
Suite *suite_moveLeft(void);
Suite *suite_shift(void);
Suite *suite_rotate(void);

Suite *suite_isCollide(void);
Suite *suite_isAttach(void);
Suite *suite_linesDisappeared(void);
Suite *suite_canSpawn(void);

Suite *suite_minor_tests(void);

void run_tests(void);
void run_testcase(Suite *testcase);

#endif  // TESTS_SUITECASES_H_
