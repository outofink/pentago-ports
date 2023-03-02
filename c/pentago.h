#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define CLEAR() printf("\033[1;1H\033[2J")
#define MIN_LETTER (int)'A'
#define MIN_NUMBER (int)'1'

typedef struct
{
  char board[4][9];
  bool turn;
} Pentago;

typedef enum direction
{
  CW = '\'',
  CCW = '\"'
} direction_t;

typedef enum pieces
{
  WHITE = 'W',
  BLACK = 'B',
  EMPTY = ' '
} pieces_t;

typedef enum winner
{
  NO_ONE,
  WHITE_WIN = 'W',
  BLACK_WIN = 'B',
  DRAW
} winner_t;

typedef char(Board)[4][9];

void initialize_game(Pentago *game);
void print_board(Board b);
void print_current_player(Pentago *game);
void switch_player(Pentago *game);
void rotate_helper(Board board, int sector, direction_t direction);
void rotate_sector(Pentago *game);
void place_piece(Pentago *game);
bool game_over(Pentago *game);
