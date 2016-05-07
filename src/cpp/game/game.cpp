#include <algorithm>
#include <assert.h>
#include <cmath>
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
// Similar to above, generate tables for all possible pairs of
// (new_row ^ old_row) after move.
Tables init_tables() {
  Tables tables;
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
    tables.score_table[row] = score;

    // by symmetry, execute a move to the left and rotate to generalize
    for (int i = 0; i < 3; ++i) {
      int j;
      for (j = i + 1; j < 4; ++j) {
        if (line[j] != 0) break;
      }
      if (j == 4) break;  // no more tiles to the right

      if (line[i] == 0) {
        line[i] = line[j];
        line[j] = 0;
        i--;  // retry this entry
      } else if (line[i] == line[j]) {
        if (line[i] != NIBBLE_MASK) {
          // pretend that 32768 + 32768 = 32768 (representational limit)
          line[i]++;
        }
        line[j] = 0;
      }
    }

    // compute all possible pairs of (new_row ^ old_row) after move
    Row result = (line[0] <<  0) |
                 (line[1] <<  4) |
                 (line[2] <<  8) |
                 (line[3] << 12);
    Row rev_result = reverse_row(result);
    unsigned rev_row = reverse_row(row);

    tables.row_left_table [    row] =                row  ^                result;
    tables.row_right_table[rev_row] =            rev_row  ^            rev_result;
    tables.col_up_table   [    row] = unpack_col(    row) ^ unpack_col(    result);
    tables.col_down_table [rev_row] = unpack_col(rev_row) ^ unpack_col(rev_result);
  }
  return tables;
}

// Get value of board by row-shifting (assume board formed by 2-tiles only).
float score_helper(Board board, const float* table) {
  return table[(board >>  0) & ROW_MASK] +
         table[(board >> 16) & ROW_MASK] +
         table[(board >> 32) & ROW_MASK] +
         table[(board >> 48) & ROW_MASK];
}

float score_board(Board board, Tables& tables) {
  return score_helper(board, tables.score_table);
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

Game play_game(ActionFunction action) {
  Board board = init_board();
  Tables tables = init_tables();

  Game game;
  game.state.push_back(board);

  int score_penalty = 0;  // "penalty" for obtaining free 4-tiles

  while (true) {
    Action move;
    Board new_board;

    // check game termination (no legal moves left)
    for (move = 0; move < NUM_MOVES; ++move) {
      if (execute_action(move, board, tables) != board) break;
    }
    if (move == NUM_MOVES) break;

    //printf("\nMove #%d, current score = %.0f\n", game.state.size(),
    //       score_board(board, tables) - score_penalty);

    move = action(board);
    if (move < 0) break;

    new_board = execute_action(move, board, tables);
    if (new_board == board) {
      printf("Illegal move!\n");
      break;  // NOTE: replace break statement with below lines when testing
      //--imove;
      //continue;
    }

    Board tile = generate_tile();
    if (tile == FOUR_TILE_NIBBLE) score_penalty += FOUR_TILE_SCORE;
    board = insert_random_tile(new_board, tile);

    game.state.push_back(board);
    game.action.push_back(move);
  }

  game.final_score = static_cast<Reward>(score_board(board, tables) - score_penalty);
  game.max_tile = static_cast<Reward>(std::pow(2, max_tile(board)));

  print_board(board);
  printf("\nGame over. Your score is %d and the maximum tile is %d.\n",
         game.final_score, game.max_tile);

  return game;
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

Board execute_up(Board board, Tables& tables) {
  Board ret = board;
  Board t = transpose(board);
  ret ^= tables.col_up_table[(t >>  0) & ROW_MASK] <<  0;
  ret ^= tables.col_up_table[(t >> 16) & ROW_MASK] <<  4;
  ret ^= tables.col_up_table[(t >> 32) & ROW_MASK] <<  8;
  ret ^= tables.col_up_table[(t >> 48) & ROW_MASK] << 12;
  return ret;
}

Board execute_down(Board board, Tables& tables) {
  Board ret = board;
  Board t = transpose(board);
  ret ^= tables.col_down_table[(t >>  0) & ROW_MASK] <<  0;
  ret ^= tables.col_down_table[(t >> 16) & ROW_MASK] <<  4;
  ret ^= tables.col_down_table[(t >> 32) & ROW_MASK] <<  8;
  ret ^= tables.col_down_table[(t >> 48) & ROW_MASK] << 12;
  return ret;
}

Board execute_left(Board board, Tables& tables) {
  Board ret = board;
  ret ^= Board(tables.row_left_table[(board >>  0) & ROW_MASK]) <<  0;
  ret ^= Board(tables.row_left_table[(board >> 16) & ROW_MASK]) << 16;
  ret ^= Board(tables.row_left_table[(board >> 32) & ROW_MASK]) << 32;
  ret ^= Board(tables.row_left_table[(board >> 48) & ROW_MASK]) << 48;
  return ret;
}

Board execute_right(Board board, Tables& tables) {
  Board ret = board;
  ret ^= Board(tables.row_right_table[(board >>  0) & ROW_MASK]) <<  0;
  ret ^= Board(tables.row_right_table[(board >> 16) & ROW_MASK]) << 16;
  ret ^= Board(tables.row_right_table[(board >> 32) & ROW_MASK]) << 32;
  ret ^= Board(tables.row_right_table[(board >> 48) & ROW_MASK]) << 48;
  return ret;
}

Board execute_action(Action action, Board board, Tables& tables) {
  switch (action) {
    case 0:
      return execute_up(board, tables);
    case 1:
      return execute_down(board, tables);
    case 2:
      return execute_left(board, tables);
    case 3:
      return execute_right(board, tables);
    default:
      return NULL_BOARD;
  }
}

