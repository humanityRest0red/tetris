/**
 * @file tetris.h
 * @author kaseltzt
 * @date 27 Apr 2024
 * @brief File containing description of functions for tetris game
 */

#ifndef BRICK_GAME_TETRIS_TETRIS_H_
#define BRICK_GAME_TETRIS_TETRIS_H_

#include <stdbool.h>

#define CELLS_IN_TETROMINO 4

#define HEIGHT 20
#define WIDTH 10

#define TETROMINO_HEIGHT 2
#define TETROMINO_WIDTH 4

#define HIGH_SCORE_FILE "high_score.txt"

typedef enum { I = 0, J, L, O, S, T, Z } TetroName_t;

/**
 * @brief Cell structure
 *
 * Cell structure with params x, y
 */
typedef struct {
  int x;
  int y;
} Cell_t;

/**
 * @brief Figure structure
 *
 * Tetromino structure
 */
typedef struct {
  Cell_t cells[CELLS_IN_TETROMINO];
  TetroName_t name;
  TetroName_t name_next;
} Tetromino_t;

/**
 * @brief States for fsm
 *
 * Game states, used as states (rows) for finite state
 * machine table
 */
typedef enum {
  START = 0,
  GAMEOVER,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
} GameState_t;

/**
 * @brief Signals for fsm
 *
 * User input, used as signals (columns) for finite state machine table
 */
typedef enum {
  Start = 0,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

/**
 * @brief Game data struct
 *
 * Game logic data
 *
 * @param field Game field
 * @param next Next spawn figure for preview
 * @param score Game score
 * @param high_score Game high score from file
 * @param level Gamel level, min 1, max 10
 * @param speed Game speed, min 1, max 10
 * @param pause Pause flag
 */
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

/**
 * @brief Game parameters struct
 *
 * Structure with all game parametres
 */
typedef struct {
  GameInfo_t info;
  Tetromino_t tetromino;
  GameState_t state;
} GameParams_t;

typedef void (*ActionT)(GameParams_t *prms);

/**
 * @brief Update current game state
 *
 * Update game state to SHIFTING
 *
 * @param prms Pointer to GameParams_t struct
 */
GameInfo_t updateCurrentState();

/**
 * @brief Update game parameters
 *
 * Update game parameters in static variable.
 *
 * @param prms Pointer to GameParams_t struct
 *
 * @return Pointer to GameParams_t struct
 */
GameParams_t *updateParams(GameParams_t *prms);

/**
 * @brief Initialize game parameters
 *
 * Allocate memory for field, set score, high_score, level, speed, pause flag
 *
 * @param prms Pointer to GameParams_t struct
 */
void initializeParams(GameParams_t *prms);

/**
 * @brief Start game
 *
 * Set game state SPAWN
 *
 * @param prms Pointer to GameParams_t struct
 */
void startGame(GameParams_t *prms);

/**
 * @brief Pause game
 *
 * Set pause status to the opposite
 *
 * @param prms Pointer to GameParams_t struct
 */
void pauseGame(GameParams_t *prms);

/**
 * @brief Terminate game
 *
 * Free allocated memory, set game state GAMEOVER
 *
 * @param prms Pointer to GameParams_t struct
 */
void terminateGame(GameParams_t *prms);

/**
 * @brief Spawn new figure
 *
 * Spawn a figure, defined in 'next'
 *
 * @param prms Pointer to GameParams_t struct
 */
void spawn(GameParams_t *prms);

/**
 * @brief Move figure down
 *
 * Move figure down by one cell
 *
 * @param prms Pointer to GameParams_t struct
 */
void moveDown(GameParams_t *prms);

/**
 * @brief Move figure right
 *
 * Move figure right by one cell
 *
 * @param prms Pointer to GameParams_t struct
 */
void moveRight(GameParams_t *prms);

/**
 * @brief Move figure left
 *
 * Move figure left by one cell
 *
 * @param prms Pointer to GameParams_t struct
 */
void moveLeft(GameParams_t *prms);

/**
 * @brief Attach figure to board cells
 *
 * Figure cells become field cells
 *
 * @param prms Pointer to GameParams_t struct
 */
void attach(GameParams_t *prms);

/**
 * @brief Shift figure down
 *
 * Shift figure down
 *
 * @param prms Pointer to GameParams_t struct
 */
void shift(GameParams_t *prms);

/**
 * @brief Rotate figure
 *
 * Rotate figure on 90 degrees right
 *
 * @param prms Pointer to GameParams_t struct
 */
void rotate(GameParams_t *prms);

/**
 * @brief Check are collided field and figure
 *
 * Check are cells under figure
 *
 * @param prms Pointer to GameParams_t struct
 *
 * @return  True or false
 */
bool isCollide(GameParams_t *prms);

/**
 * @brief Check is attach
 *
 * Check are cells under figure
 *
 * @param prms Pointer to GameParams_t struct
 *
 * @return  True or false
 */
bool isAttach(GameParams_t *prms);

/**
 * @brief Remove lines
 *
 * Find for each line is it filled, count removed lines
 *
 * @param field Pointer to tetris field
 *
 * @return Removed lines count
 */
int linesDisappeared(int **field);

/**
 * @brief Check can cpawn
 *
 * Check is free space for next figure
 *
 * @param prms Pointer to GameParams_t struct
 *
 * @return True of false
 */
bool canSpawn(GameParams_t *prms);

/**
 * @brief Allocate matrix
 *
 * Allocate memory array of pointers, then memory for each pointer
 *
 * @param n Rows count
 * @param m Columns count
 *
 * @return Pointer to matrix
 */
int **newArray2D(int n, int m);

/**
 * @brief Free matrix
 *
 * Free pointers, then free pointer to array of pointers
 *
 * @param this Matrix
 * @param n Rows count
 */
void deleteArray2D(int **this, int n);

/**
 * @brief Read high score
 *
 * Read number from defined file
 *
 * @return Read number
 */
int readHighScore(void);

/**
 * @brief Write high score
 *
 * Write number in defined file
 *
 * @param high_score New high score
 */
void writeHighScore(int high_score);

#endif  // BRICK_GAME_TETRIS_TETRIS_H_