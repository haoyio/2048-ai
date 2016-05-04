#ifndef GAME_H_
#define GAME_H_

#include <stdio.h>
#include <vector>

#include "../defines.h"

struct Game {
  std::vector<Board> state;
  std::vector<Action> action;
  std::vector<Reward> reward;  // score added per move
  Reward score;
};

struct Tables {
  float score_table[MAX_ROW];
  Row row_left_table[MAX_ROW];
  Row row_right_table[MAX_ROW];
  Board col_up_table[MAX_ROW];
  Board col_down_table[MAX_ROW];
};

inline Board unpack_col(Row row) {
  Board tmp = row;
  return (tmp | (tmp << 12ULL) | (tmp << 24ULL) | (tmp << 36ULL)) & COL_MASK;
}

inline Row reverse_row(Row row) {
  return (row >> 12) | ((row >> 4) & 0x00F0) | ((row << 4) & 0x0F00) | (row << 12);
}

// 90% chance of generating a 2^1 tile, 10% a 2^2 tile.
inline Board generate_tile() {
  return (unif_random(10) < 9) ? 1 : 2;
}

Tables init_tables();
float score_helper(Board board, const float* table);
float score_board(Board board, Tables& tables);

Board init_board();
Board insert_random_tile(Board board, Board tile);
void play_game(ActionFunction action);
void print_board(Board board);

int max_tile(Board board);
int count_empty(Board board);
Board transpose(Board board);

Board execute_up(Board board, Tables& tables);
Board execute_down(Board board, Tables& tables);
Board execute_left(Board board, Tables& tables);
Board execute_right(Board board, Tables& tables);
Board execute_action(Action action, Board board, Tables& tables);

#endif  // GAME_H_
