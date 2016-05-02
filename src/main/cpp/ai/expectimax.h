#ifndef EXPECTIMAX_H
#define EXPECTIMAX_H

static const Board COL_MASK = 0x000F000F000F000FULL;

inline Board unpack_col(Row row) {
  Board tmp = row;
  return (tmp | (tmp << 12ULL) | (tmp << 24ULL) | (tmp << 36ULL)) & COL_MASK;
}

#endif  // EXPECTIMAX_H
