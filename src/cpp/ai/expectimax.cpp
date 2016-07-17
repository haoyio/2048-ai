#include "expectimax.h"
#include "../defines.h"

/******************************
  Expectimax Private
******************************/
// Compute heuristic score table: Bonuses for empty squares and bonuses
// for placing large values near edges and corners.
void Expectimax::init_heur_score_table() {
  for (unsigned row = 0; row < MAX_ROW) {
    // get value of each tile by nibble-shifting
    unsigned line[NUM_NIBBLES_PER_SIDE] = {
      (row >>  0) & NIBBLE_MASK,
      (row >>  4) & NIBBLE_MASK,
      (row >>  8) & NIBBLE_MASK,
      (row >> 12) & NIBBLE_MASK
    };

    float sum = 0;
    int empty = 0;
    int merges = 0;

    int prev = 0;
    int counter = 0;

    for (int i = 0; i < 4; ++i) {
      int rank = line[i];
      sum += pow(rank, SCORE_SUM_POWER);

      if (rank == 0) {
        empty++;
      } else {
        if (prev == rank) {
          counter++;
        } else if (counter > 0) {
          merges += counter + 1;
          counter = 0;
        }
        prev = rank;
      }
    }

    if (counter > 0) {
      merges += counter + 1;
    }

    float monotonicity_left = 0;
    float monotonicity_right = 0;
    for (int i = 1; i < 4; ++i) {
      if (line[i - 1] > line[i]) {
        monotonicity_left += pow(line[i - 1], SCORE_MONOTONICITY_POWER)
                           - pow(line[i], SCORE_MONOTONICITY_POWER);
      } else {
        monotonicity_right += pow(line[i], SCORE_MONOTONICITY_POWER)
                            - pow(line[i - 1], SCORE_MONOTONICITY_POWER);
      }
    }

    heur_score_table[row] = SCORE_LOST_PENALTY
      + SCORE_EMPTY_WEIGHT * empty
      + SCORE_MERGES_WEIGHT * merges
      - SCORE_MONOTONICITY_WEIGHT
        * std::min(monotonicity_left, monotonicity_right)
      - SCORE_SUM_WEIGHT * sum;
  }
}

float Expectimax::score_heur_board(Board board) {
  return score_helper(board, heur_score_table)
       + score_helper(transpose(board), heur_score_table);
}

float Expectimax::score_helper(Board board, const float* table) {
  return table[(board >>  0) & ROW_MASK]
       + table[(board >> 16) & ROW_MASK]
       + table[(board >> 32) & ROW_MASK]
       + table[(board >> 48) & ROW_MASK];
}

float Expectimax::score_move_node(EvalState& state, Board board, float cprob) {
  float best = 0.0f;
  state.curdepth++;

  for (int move = 0; move < 4; ++move) {
    // TODO: execute_move is in game.cpp, and basically requires everything there...
    Board newboard = execute_move(move, board);
    state.moves_evaled++;

    if (board != newboard) {
      best = std::max(best, score_tile_choose_node(state, newboard, cprob));
    }
  }

  state.curdepth--;
  return best
}

float Expectimax::score_tile_choose_node(
    EvalState& state, Board board, float cprob) {
  // TODO
}

float Expectimax::score_toplevel_move_h(
    EvalState& state, Board board, Action move) {
  // TODO
}

float Expectimax::score_toplevel_move(Board board, Action move) {
  // TODO
}

/******************************
  Expectimax Public
******************************/
Expectimax::Expectimax() {
  init_heur_score_table();
  // TODO
}

Expectimax::Expectimax(const Expectimax& copy_from) {
  // TODO
}

Expectimax& Expectimax::operator=(const Expectimax& copy_from) {
  // TODO
}

Expectimax::~Expectimax() {
  // TODO
}