#include "board.cpp"
#include "player.cpp"
#include "input_helper.h"
#include <chrono>
#include <thread>
using namespace std;
int main (void) {
  board_drawer board = board_drawer();
  bool game_active = true;
  int counter = 0;
  string input;
  cout << "Enter Player 1's name: ";
  cin >> input;
  player players[2] = {player(input, 'x'),  player("Player 2", 'o')};
  while (game_active) {
    board.redraw_board();
    if (counter % 2 == 0) {
      bool invalid_input = true;
      int mark = 0;
      while (invalid_input) {
        cout << players[0] << " please choose a position to mark" << endl;
        cin >> mark;
        cout << endl;
        try {
          if (input_helper::is_pos_number(std::to_string(mark)))
          {
            std::vector<int> valid_pos = board.get_unused_board_positions();
            if (not std::count(valid_pos.begin(), valid_pos.end(), mark-1)) {
              cout << "Invalid input! please choose a valid position on the board!\n";
              continue;
            }
            board.mark_board(mark, 'x');
            invalid_input = false;
          }
          else {
            cout << "Invalid input! please choose a position on the board!\n";
          }
        } catch (std::out_of_range) {
          cout << "Invalid input! please choose a position on the board!\n";
        }
      }
    } else {
      this_thread::sleep_for(chrono::milliseconds(500));
      int mark = players[1].choose_random_number(board.get_unused_board_positions());
      board.mark_board(mark, 'o');
    }
    // board.mark_board(mark, 'x');
    if (board.check_win() == 1) {
      board.redraw_board();
      //print the name of the current player who won
      cout << players[counter % 2] << " Won!" << endl;
      game_active = false;
    }
    else if (board.check_win() == 0) {
      cout << "no winner could be determined!" << endl;
    }
    else {
      counter++;
    }
  }
  return 0;
};