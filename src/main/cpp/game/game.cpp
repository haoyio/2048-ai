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

// TODO: implement the following
/*
Board execute_up(Board board);
Board execute_down(Board board);
Board execute_left(Board board);
Board execute_right(Board board);
Board execute_action(Action action, Board board);

int max_tile(Board board);
int distinct_tiles(Board board);
*/

// Count the number of empty positions (= zero nibbles) in a board.
// Assumption: Board is not empty (makes sense because of initialization).
// Courtesy: kcwu
int count_empty(Board board) {
  board = ~board;
  board &= board >> 2;
  board &= board >> 1;
  board &= 0x1111111111111111ULL;
  board = (board * 0x1111111111111111ULL) >> 60;
  return board;
}

// Transpose rows/columns in a board:
//   0123       048c
//   4567  -->  159d
//   89ab       26ae
//   cdef       37bf
// Courtesy: kcwu
Board transpose(Board board) {
  Board tmp;
  tmp = (board ^ (board >> 12)) & 0x0000F0F00000F0F0ULL;
  board ^= tmp ^ (tmp << 12);
  tmp = (board ^ (board >> 24)) & 0x00000000FF00FF00ULL;
  board ^= tmp ^ (tmp << 24);
  return board;
}

// TODO: void init_score_table();

Board init_board() {
  Board board = generate_tile() << (4 * unif_random(16));
  return insert_random_tile(board, draw_tile());
}

Board insert_random_tile(Board board, Board tile) {
  int index = unif_random(count_empty(board));
  Board tmp = board;
  while (true) {
    while ((tmp & 0xF) != 0) {
      tmp >>= 4;
      tile <<= 4;
    }
    if (index == 0) break;
    --index;
    tmp >>= 4;
    tile <<= 4;
  }
  return board | tile;
}

void play_game(ActionFunction action) {
  Board board = initial_board();
  int imove = 0;
  int score_penalty = 0;  // "penalty" for obtaining free 4-tiles

  while (true) {
    Action move;
    Board new_board;

    // check game termination (no legal moves left)
    for (move = 0; move < NUM_MOVES; ++move) {
      if (execute_action(move, board) != board) break;
    }
    if (move == NUM_MOVES) break;

    printf("\nMove #%d, current score = %.0f\n", ++imove,
           score_board(board) - score_penalty);

    move = action(board);
    if (move < 0) break;

    new_board = execute_action(move, board);
    if (new_board == board) {
      printf("Illegal move!\n");
      --imove;
      continue;
    }

    Board tile = generate_tile();
    if (tile == FOUR_TILE_NIBBLE) score_penalty += FOUR_TILE_SCORE;
    board = insert_random_tile(new_board, tile);
  }

  print_board(board);
  printf("\nGame over. Your score is %.0f and the maximum tile is %d.\n",
         score_board(board) - score_penalty, max_tile(board));
}

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

