#ifndef DEFINES_H_
#define DEFINES_H_

#include <cstdlib>
#include <ctime>

#include <stdlib.h>

// The 4x4 board is represented as a 64-bit word, where each tile packed into
// a single 4-bit nibble. Because of this limitation, the maximum tile value is
// 32768 (2^15), but this is a minor limitation as achieving 65536 is highly
// unlikely under normal circumstances. It'll be easy to extend this to larger
// tiles once everything is running properly, anyways.

// The nibble shift can be computed as (r,c) -> shift (4*r + c), where (0,0) is
// the LSB.

#define NUM_MOVES 4  // 0 = up, 1 = down, 2 = left, 3 = right
#define MAX_ROW 65536

#define NUM_TILES_PER_BOARD 16
#define NUM_NIBBLES_PER_SIDE 4
#define NIBBLE_SHIFT 4

#define NIBBLE_MASK 0xF
#define ROW_MASK 0xFFFFULL
#define COL_MASK 0x000F000F000F000FULL

#define FOUR_TILE_NIBBLE 2  // nibble representation of 4-tile (2^2)
#define FOUR_TILE_SCORE 4

#define NULL_BOARD ~0ULL

typedef uint64_t Board;
typedef uint16_t Row;
typedef uint32_t Reward;
typedef int Action;

typedef Action (*ActionFunction)(Board);

// Uniformly random selection of an integer between 0 and n - 1.
inline unsigned unif_random(unsigned n) {
  std::srand(time(NULL));
  return std::rand() % n;
}

#endif  // DEFINES_H_
