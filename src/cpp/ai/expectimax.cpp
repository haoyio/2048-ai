#include "expectimax.h"
#include "../defines.h"

void init_heur_score_table();
float score_heur_board(Board board);
float score_helper(Board board, const float* table);
float score_move_node(EvalState& state, Board board, float cprob);
float score_tile_choose_node(EvalState& state, Board board, float cprob);
float score_toplevel_move_h(EvalState& state, Board board, Action move);
float score_toplevel_move(Board board, Action move);

Expectimax::Expectimax() {
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

