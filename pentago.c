#include "pentago.h"

void initialize_game (Pentago *game)
{
  memset (game->board, EMPTY, 36);
  game->turn = WHITE;
}

void print_board (Board b)
{
  CLEAR();
  printf ("%c %c %c|%c %c %c\n%c %c %c|%c %c %c\n%c %c %c|%c %c %c\n"
          "-----+-----\n"
          "%c %c %c|%c %c %c\n%c %c %c|%c %c %c\n%c %c %c|%c %c %c\n\n",
          b[0][0], b[0][1], b[0][2], b[1][0], b[1][1], b[1][2],
          b[0][3], b[0][4], b[0][5], b[1][3], b[1][4], b[1][5],
          b[0][6], b[0][7], b[0][8], b[1][6], b[1][7], b[1][8],
          b[2][0], b[2][1], b[2][2], b[3][0], b[3][1], b[3][2],
          b[2][3], b[2][4], b[2][5], b[3][3], b[3][4], b[3][5],
          b[2][6], b[2][7], b[2][8], b[3][6], b[3][7], b[3][8]);
}

void print_current_player (Pentago *game)
{
  char *turn = game->turn ? "White" : "Black";
  printf ("%s's turn:\n", turn);
}

char get_piece_from_player (bool player)
{
  return player ? WHITE : BLACK;
}

void switch_player (Pentago *game)
{
  game->turn = !game->turn;
}

void rotate_helper (Board board, int sector, direction_t direction)
{
  if (direction == CW)
  {
    char temp = board[sector][0];
    board[sector][0] = board[sector][6];
    board[sector][6] = board[sector][8];
    board[sector][8] = board[sector][2];
    board[sector][2] = temp;
    temp = board[sector][1];
    board[sector][1] = board[sector][3];
    board[sector][3] = board[sector][7];
    board[sector][7] = board[sector][5];
    board[sector][5] = temp;
  }
  else
  {
    char temp = board[sector][0];
    board[sector][0] = board[sector][2];
    board[sector][2] = board[sector][8];
    board[sector][8] = board[sector][6];
    board[sector][6] = temp;
    temp = board[sector][1];
    board[sector][1] = board[sector][5];
    board[sector][5] = board[sector][7];
    board[sector][7] = board[sector][3];
    board[sector][3] = temp;
  }
}

void rotate_sector (Pentago *game)
{
  char input[3];
  while (1)
  {
    print_board (game->board);
    print_current_player (game);
    printf ("Rotation (ie. A' or C\"): ");
    fgets (input, 3, stdin);
    if (input[2] != '\0' || input[0] < 'A' || input[0] > 'D' ||
        (input[1] != CW && input[1] != CCW))
    {
      continue;
    }
    break;
  }
  int sector = input[0] - MIN_LETTER;
  char direction = input[1];
  rotate_helper (game->board, sector, direction);
  printf ("\n");
}

void place_piece (Pentago *game)
{
  char input[3];
  int sector, position;
  while (1)
  {
    print_board (game->board);
    print_current_player (game);
    printf ("Location (ie. A4 or C9): ");
    fgets (input, 3, stdin);
    if (input[2] != '\0' ||
        input[0] < 'A' || input[0] > 'D' ||
        input[1] < '1' || input[1] > '9')
    {
      continue;
    }
    sector = input[0] - MIN_LETTER;
    position = input[1] - MIN_NUMBER;
    if (game->board[sector][position] != EMPTY)
    {
      continue;
    }
    break;
  }
  game->board[sector][position] = get_piece_from_player (game->turn);
  printf ("\n");
}

bool game_over (Pentago *game)
{
  winner_t winner = NO_ONE;
  char win_arrays[18][6] = {
      // columns
      {0,  3,  6,  18, 21, 24},
      {1,  4,  7,  19, 22, 25},
      {2,  5,  8,  20, 23, 26},
      {9,  12, 15, 27, 30, 33},
      {10, 13, 16, 28, 31, 34},
      {11, 14, 17, 29, 32, 35},
      // rows
      {0,  1,  2,  9,  10, 11},
      {3,  4,  5,  12, 13, 14},
      {6,  7,  8,  15, 16, 17},
      {18, 19, 20, 27, 28, 29},
      {21, 22, 23, 30, 31, 32},
      {24, 25, 26, 33, 34, 35},
      // diagonals
      {0,  4,  8,  27, 31, 35},
      {11, 13, 15, 20, 22, 24},
      {10, 12, 8,  19, 21},
      {14, 16, 27, 23, 25},
      {1,  5,  15, 28, 31},
      {3,  7,  20, 30, 34},
  };
  for (int i = 0; i < 18; i++)
  {
    int count = 0;
    char last = EMPTY;
    int until = i < 14 ? 6 : 5;
    for (int j = 0; j < until; j++)
    {
      char piece = game->board[win_arrays[i][j] / 9][win_arrays[i][j] % 9];
      if (piece == EMPTY)
        continue;
      else if (piece == last)
        count++;
      else
      {
        count = 1;
        last = piece;
      }
      if (count >= 5)
      {
        if (winner == NO_ONE)
          winner = last == WHITE ? WHITE_WIN : BLACK_WIN;
        else if (winner != (winner_t) last)
          winner = DRAW;
        break;
      }
    }
  }
  if (winner == NO_ONE)
    return false;
  CLEAR();
  print_board (game->board);
  if (winner == WHITE_WIN)
    printf ("White wins!\n");
  else if (winner == BLACK_WIN)
    printf ("Black wins!\n");
  else
    printf ("Draw!\n");
  return true;
}

int main ()
{
  Pentago game;
  initialize_game (&game);
  while (1)
  {
    place_piece (&game);
    if (game_over (&game))
      break;
    rotate_sector (&game);
    if (game_over (&game))
      break;
    switch_player (&game);
  }
  return 0;
}
