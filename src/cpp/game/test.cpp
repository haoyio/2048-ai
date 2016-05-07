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
  char move;
  std::cout << "Please enter w (up), s (down), a (left), or d (right): ";
  std::cin >> move;
  std::cout << "You input: ";
  switch (move) {
    case 'w':
      std::cout << "up\n";
      return 0;
    case 's':
      std::cout << "down\n";
      return 1;
    case 'a':
      std::cout << "left\n";
      return 2;
    case 'd':
      std::cout << "right\n";
      return 3;
    default:
      std::cout << move << "\nInvalid character, ending game...\n";
      return -1;
  }
}

void test_game() {
  play_game(user_move);
}

int main() {
  test_print();
  test_game();

  return 0;
}