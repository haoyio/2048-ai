#include <iostream>
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

Action user_move(Board board) {
  print_board(board);
  int move;
  std::cout << "Please enter 0 (up), 1 (down), 2 (left), or 3 (right): ";
  std::cin >> move;
  std::cout << "You input ";
  switch (move) {
    case 0:
      std::cout << "up\n";
      return 0;
    case 1:
      std::cout << "down\n";
      return 1;
    case 2:
      std::cout << "left\n";
      return 2;
    case 3:
      std::cout << "right\n";
      return 3;
    default:
      std::cout << "an invalid character, ending game...\n";
      return -1;
  }
}

void test_game() {
  play_game(user_move);
}

int main() {
  test_game();

  return 0;
}