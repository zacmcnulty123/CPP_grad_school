#include "board.cpp"
using namespace std;
int main (void) {
  board_drawer board = board_drawer();
  bool winner_found = false;
  while (not winner_found) {
    board.redraw_board();
    cout << "please choose a position to mark" << endl;
    int mark = 0;
    cin >> mark;
    board.mark_board(mark, 'x');
    board.redraw_board();
    winner_found = true;
  }
  return 0;
};