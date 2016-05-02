#ifndef GAME_H
#define GAME_H

#include <stdio.h>

#include "../defines.h"

static const Board ROW_MASK = 0xFFFFULL;

struct Game {
  std::vector<Board> state;
  std::vector<Action> action;
  std::vector<Reward> reward;  // score added per move
  Reward score;
};

inline Row reverse_row(Row row) {
  return (row >> 12) | ((row >> 4) & 0x00F0) | ((row << 4) & 0x0F00) | (row << 12);
}

inline generate_tile() {
  return (unif_random(10) < 9) ? 1 : 2;
}

Board execute_up(Board board);
Board execute_down(Board board);
Board execute_left(Board board);
Board execute_right(Board board);
Board execute_action(Action action, Board board);

int max_tile(Board board);
int distinct_tiles(Board board);
int count_empty(Board board);
Board transpose(Board board);

void init_score_table();
Board init_board();
Board insert_random_tile(Board board, Board tile);
void play_game(ActionFunction action);
void print_board(Board board);

#endif  // GAME_H
