#ifndef EXPECTIMAX_H
#define EXPECTIMAX_H

#include <unordered_map>

#include "../defines.h"

// heuristic score parameters
#define SCORE_LOST_PENALTY 200000.0f
#define SCORE_MONOTONICITY_POWER 4.0f
#define SCORE_MONOTONICITY_WEIGHT 47.0f
#define SCORE_SUM_POWER 3.5f
#define SCORE_SUM_WEIGHT 11.0f
#define SCORE_MERGES_WEIGHT 700.0f
#define SCORE_EMPTY_WEIGHT 270.0f

#define CPROB_THRESH_BASE 0.0001f
#define CACHE_DEPTH_LIMIT 15

struct TransTableEntry {
  uint8_t depth;
  float heuristic;
};

typedef std::unordered_map<Board, TransTableEntry> TransTable;

struct EvalState {
  TransTable trans_table;  // transposition table to cache seen moves
  int max_depth;
  int curr_depth;
  int cache_hits;
  unsigned long moves_evaled;
  int depth_limit;

  EvalState(): max_depth(0), curr_depth(0), cache_hits(0),
               moves_evaled(0), depth_limit(0) {}
};

class Expectimax {
  private:
    float heur_score_table[MAX_ROW];

  public:
    Expectimax();
    Expectimax(const Expectimax& copy_from);
    Expectimax& operator=(const Expectimax& copy_from);
    ~Expectimax();

    Action expectimax_move(Board board);
};

#endif  // EXPECTIMAX_H
