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

inline Row reverse_row(Row row) {
  return (row >> 12) | ((row >> 4) & 0x00F0) | ((row << 4) & 0x0F00) | (row << 12);
}

// 90% chance of generating a 2^1 tile, 10% a 2^2 tile.
inline Board generate_tile() {
  return (unif_random(10) < 9) ? 1 : 2;
}

void init_score_table();
float score_helper(Board board, const float* table);
float score_board(Board board);

Board init_board();
Board insert_random_tile(Board board, Board tile);
void play_game(ActionFunction action);
void print_board(Board board);

int max_tile(Board board);
int count_empty(Board board);
Board transpose(Board board);

Board execute_up(Board board);
Board execute_down(Board board);
Board execute_left(Board board);
Board execute_right(Board board);
Board execute_action(Action action, Board board);

#endif  // GAME_H_
