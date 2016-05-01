#ifndef GAME_H
#define GAME_H

#include <stdio.h>

#include "../defines.h"

static const Board ROW_MASK = 0xFFFFULL;
static const Board COL_MASK = 0x000F000F000F000FULL;

struct Game {
  std::vector<Board> state;
  std::vector<Action> action;
  std::vector<Reward> reward;  // score added per move
  Reward score;
};

Board unpack_col(Row row);
Row reverse_row(Row row);

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

#endif  // GAME_H
