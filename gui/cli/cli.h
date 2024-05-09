/**
 * @file cli.h
 * @author kaseltzt
 * @date 27 Apr 2024
 * @brief File containing description of functions gui cli tetris game
 */

#ifndef BRICK_GAME_GUI_CLI_H_
#define BRICK_GAME_GUI_CLI_H_

#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include "../../brick_game/tetris/tetris.h"

#define COLOR_ORANGE 8

#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)

#define ESCAPE_KEY 27
#define ENTER_KEY 10
#define PAUSE_KEY 112
#define ACTION_KEY 32

#define INTRO_MESSAGE "Press ENTER to start"
#define INTRO_MESSAGE_LEN 21

#define BOARDS_BEGIN 2

#define BOARD_N HEIGHT
#define BOARD_M 2 * WIDTH

#define HUD_WIDTH 16

#define Y_NEXT 2
#define Y_SCORE 9
#define Y_HIGH_SCORE 14
#define Y_LEVEL 19

/**
 * @brief Win init
 *
 * Init ncurses win
 */
void myWinInit();

/**
 * @brief Game loop
 *
 * Cycle of game
 */
void gameLoop();

/**
 * @brief Change game state depending on user input
 *
 * Change game state depending on user input using FSM
 *
 * @param action Action type value
 * @param hold Bool value of was key hold
 */
void userInput(UserAction_t action, bool hold);

/**
 * @brief Get user action
 *
 * Get user action enum value depending on pressed button
 */
UserAction_t getAction(int user_input);

/**
 * @brief Draw GUI
 *
 * Draw static part of GUI
 */
void drawGui();

/**
 * @brief Draw rectangle
 *
 * Draw rectangle
 *
 * @param top_y Top y value
 * @param bottom_y Bottom y value
 * @param left_x Left x value
 * @param right_x Right x value
 */
void drawRectangle(int top_y, int bottom_y, int left_x, int right_x);

/**
 * @brief Draw game info
 *
 * Draw next figure, score, high score, level, speed
 *
 * @param info Game logic data struct
 * @param type_next Type of next figure
 */
void drawInfo(GameInfo_t *info, int type_next);

/**
 * @brief Draw game over info
 *
 * Draw game over info
 */
void drawGameOver();

/**
 * @brief Draw pause info
 *
 * Draw pause info
 */
void drawPause();

/**
 * @brief Draw figure
 *
 * Draw tetromino
 *
 * @param tetromino Pointer on figure
 */
void drawTetromino(Tetromino_t *tetromino);

/**
 * @brief Draw game field
 *
 * Draw game field
 * @param field 2D array of cells values
 * @param colors 2D array of cells colors
 */
void drawField(int **field, int **colors);

/**
 * @brief Draw cell
 *
 * Draw cell
 * @param i Vertical coord value
 * @param j Horizontal coord value
 * @param isCell Flag is cell
 * @param color Color of cell
 */
void drawCell(int i, int j, bool isCell, int color);

#endif  // BRICK_GAME_GUI_CLI_H_