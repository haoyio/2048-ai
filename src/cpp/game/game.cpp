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

// Generate a table that gives the score for any row (or transposed col).
// Idea: Precompute the score by assuming a given row is formed entirely by
// 2-tiles. Correcting for 4-tiles generated is done at runtime by keeping
// track of the number of 4-tiles generated so far and subtracting it away from
// the sum of the scores of each row for any given board.
float score_table[MAX_ROW];

// Similar to above, generate tables for all possible pairs of
// (new_row ^ old_row) after move.
Row row_left_table[MAX_ROW];
Row row_right_table[MAX_ROW];
Board col_up_table[MAX_ROW];
Board col_down_table[MAX_ROW];

void init_tables() {
  for (unsigned row = 0; row < MAX_ROW; ++row) {
    // get value of each tile by nibble-shifting
    unsigned line[NUM_NIBBLES_PER_SIDE] = {
      (row >>  0) & NIBBLE_MASK,
      (row >>  4) & NIBBLE_MASK,
      (row >>  8) & NIBBLE_MASK,
      (row >> 12) & NIBBLE_MASK
    };

    // compute score table
    float score = 0.0f;
    for (int i = 0; i < NUM_NIBBLES_PER_SIDE; ++i) {
      int tile = line[i];
      if (tile >= 2) {
        score += (tile - 1) * (1 << tile);
      }
    }
    score_table[row] = score;

    // compute all possible pairs of (new_row ^ old_row) after move
    Row result = (line[0] <<  0) |
                 (line[1] <<  4) |
                 (line[2] <<  8) |
                 (line[3] << 12);
    Row rev_result = reverse_row(result);
    unsigned rev_row = reverse_row(row);

    row_left_table [    row] =                row  ^                result;
    row_right_table[rev_row] =            rev_row  ^            rev_result;
    col_up_table   [    row] = unpack_col(    row) ^ unpack_col(    result);
    col_down_table [rev_row] = unpack_col(rev_row) ^ unpack_col(rev_result);
  }
}

// Get value of board by row-shifting (assume board formed by 2-tiles only).
float score_helper(Board board, const float* table) {
  return table[(board >>  0) & ROW_MASK] +
         table[(board >> 16) & ROW_MASK] +
         table[(board >> 32) & ROW_MASK] +
         table[(board >> 48) & ROW_MASK];
}

float score_board(Board board) {
  return score_helper(board, score_table);
}

Board init_board() {
  Board board = generate_tile() <<
    (NIBBLE_SHIFT * unif_random(NUM_TILES_PER_BOARD));
  return insert_random_tile(board, generate_tile());
}

Board insert_random_tile(Board board, Board tile) {
  int index = unif_random(count_empty(board));
  Board tmp = board;
  while (true) {
    while ((tmp & NIBBLE_MASK) != 0) {
      tmp >>= NIBBLE_SHIFT;
      tile <<= NIBBLE_SHIFT;
    }
    if (index == 0) break;
    --index;
    tmp >>= NIBBLE_SHIFT;
    tile <<= NIBBLE_SHIFT;
  }
  return board | tile;
}

void play_game(ActionFunction action) {
  Board board = init_board();
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
  for (i = 0; i < NUM_NIBBLES_PER_SIDE; ++i) {
    for (j = 0; j < NUM_NIBBLES_PER_SIDE; ++j) {
      powerval = board & NIBBLE_MASK;
      printf("%6u", (powerval == 0) ? 0 : 1 << powerval);
      board >>= NIBBLE_SHIFT;
    }
    printf("\n");
  }
  printf("\n");
}

int max_tile(Board board) {
  int maxtile = 0;
  while (board) {
    maxtile = std::max(maxtile, int(board & NIBBLE_MASK));
    board >>= NIBBLE_SHIFT;
  }
  return maxtile;
}

// Count the number of empty positions (= zero nibbles) in a board.
// Assumption: Board is not empty (makes sense because of initialization).
// Bit hack courtesy: kcwu
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
// Bit hack courtesy: kcwu
Board transpose(Board board) {
  Board tmp;
  tmp = (board ^ (board >> 12)) & 0x0000F0F00000F0F0ULL;
  board ^= tmp ^ (tmp << 12);
  tmp = (board ^ (board >> 24)) & 0x00000000FF00FF00ULL;
  board ^= tmp ^ (tmp << 24);
  return board;
}

Board execute_up(Board board) {
  Board ret = board;
  Board t = transpose(board);
  ret ^= col_up_table[(t >>  0) & ROW_MASK] <<  0;
  ret ^= col_up_table[(t >> 16) & ROW_MASK] <<  4;
  ret ^= col_up_table[(t >> 32) & ROW_MASK] <<  8;
  ret ^= col_up_table[(t >> 48) & ROW_MASK] << 12;
  return ret;
}

Board execute_down(Board board) {
  Board ret = board;
  Board t = transpose(board);
  ret ^= col_down_table[(t >>  0) & ROW_MASK] <<  0;
  ret ^= col_down_table[(t >> 16) & ROW_MASK] <<  4;
  ret ^= col_down_table[(t >> 32) & ROW_MASK] <<  8;
  ret ^= col_down_table[(t >> 48) & ROW_MASK] << 12;
  return ret;
}

Board execute_left(Board board) {
  Board ret = board;
  ret ^= Board(row_left_table[(board >>  0) & ROW_MASK]) <<  0;
  ret ^= Board(row_left_table[(board >> 16) & ROW_MASK]) << 16;
  ret ^= Board(row_left_table[(board >> 32) & ROW_MASK]) << 32;
  ret ^= Board(row_left_table[(board >> 48) & ROW_MASK]) << 48;
  return ret;
}

Board execute_right(Board board) {
  Board ret = board;
  ret ^= Board(row_right_table[(board >>  0) & ROW_MASK]) <<  0;
  ret ^= Board(row_right_table[(board >> 16) & ROW_MASK]) << 16;
  ret ^= Board(row_right_table[(board >> 32) & ROW_MASK]) << 32;
  ret ^= Board(row_right_table[(board >> 48) & ROW_MASK]) << 48;
  return ret;
}

Board execute_action(Action action, Board board) {
  switch (action) {
    case 0:
      return execute_up(board);
    case 1:
      return execute_down(board);
    case 2:
      return execute_left(board);
    case 3:
      return execute_right(board);
    default:
      return NULL_BOARD;
  }
}

