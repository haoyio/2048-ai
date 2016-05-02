#ifndef DEFINES_H
#define DEFINES_H

#include <stdlib.h>

#define NUM_MOVES 4  // up, down, left, right

#define FOUR_TILE_NIBBLE 2  // nibble representation of 4-tile (2^2)
#define FOUR_TILE_SCORE 4

// The 4x4 board is represented as a 64-bit word, where each tile packed into
// a single 4-bit nibble. Because of this limitation, the maximum tile value is
// 32768 (2^15), but this is a minor limitation as achieving 65536 is highly
// unlikely under normal circumstances. It'll be easy to extend this to larger
// tiles once everything is running properly, anyways.

// The nibble shift can be computed as (r,c) -> shift (4*r + c), where (0,0) is
// the LSB.

typedef uint64_t Board;
typedef uint16_t Row;
typedef uint32_t Reward;
typedef int Action;

typedef Action (*ActionFunction)(Board);

// Uniformly random selection of an integer between 0 and n - 1.
static inline unif_random(unsigned n) {
  return std::rand() % n;
}

#endif  // DEFINES_H
