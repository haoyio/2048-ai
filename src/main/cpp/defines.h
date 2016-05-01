#ifndef DEFINES_H
#define DEFINES_H

// The 4x4 board is represented as a 64-bit word, where each tile packed into
// a single 4-bit nibble. Because of this limitation, the maximum tile value is
// 32768 (2^15), but this is a minor limitation as achieving 65536 is highly
// unlikely under normal circumstances. It'll be easy to extend this to larger
// tiles once everything is running properly, anyways.

// The nibble shift can be computed as (r,c) -> shift (4*r + c), where (0,0) is
// the LSB.

enum Action { UP, DOWN, LEFT, RIGHT };

typedef uint64_t Board;
typedef uint16_t Row;
typedef uint32_t Reward;

typedef Action (*ActionFunction)(Board);

#endif  // DEFINES_H
