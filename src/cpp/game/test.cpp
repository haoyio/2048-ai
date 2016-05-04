#include <vector>

#include "../defines.h"
#include "game.h"

void test_print() {
  std::vector<Board> boards = {
    0x6621533332211111ull, 0x8643553333101100ull, 0x9653531032001001ull,
   0x01231334047814cdull, 0x23212234346935cdull, 0x12231334248916cdull,
    0x10022321345a34cdull, 0x11222234148a11cdull, 0x21121667028a20cdull
  };

  for (auto it = boards.begin(); it != boards.end(); ++it) {
    print_board(*it);
  }
}

int main() {
  test_print();

  return 0;
}