#include <algorithm>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game.h"
#include "../defines.h"

Board unpack_col(Row row) {
  Board tmp = row;
  return (tmp | (tmp << 12ULL) | (tmp << 24ULL) | (tmp << 36ULL)) & COL_MASK;
}

Row reverse_row(Row row) {
  return (row >> 12) | ((row >> 4) & 0x00F0) | ((row << 4) & 0x0F00) | (row << 12);
}

// TODO: implement the following
/*
Board execute_up(Board board);
Board execute_down(Board board);
Board execute left(Board board);
Board execute right(Board board);
Board execute_action(Action action, Board board);

int max_tile(Board board);
int distinct_tiles(Board board);

Board initial_board();
Board insert_random_tile(Board board, Board tile);
void play_game(ActionFunction actionFunction);
void print_board(Board board);
*/

void print_board(Board board) {
  int i, j, powerval;
  for (i = 0; i < 4; ++i) {
    for (j = 0; j < 4; ++j) {
      powerval = (board) & 0xf;
      printf("%6f", (powerval == 0) ? 0 : 1 << powerval);
      board >>= 4;
    }
    printf("\n");
  }
  printf("\n");
}

