#include "board.h"
#include "input_helper.h"
board_drawer::board_drawer() {

}
board_drawer::~board_drawer() {}
void board_drawer::redraw_board() {
  system("cls");
  std::cout << title << vert_bars
  << " " << board_vals[0] << " | " << board_vals[1] << " | " << board_vals[2]
  << std::endl <<  section_break << vert_bars
  << " " << board_vals[3] << " | " << board_vals[4] << " | " << board_vals[5]
  << std::endl <<  section_break << vert_bars
  << " " << board_vals[6] << " | " << board_vals[7] << " | " << board_vals[8]
  << std::endl <<  section_break << vert_bars;

}

bool board_drawer::mark_board(int loc, char mark) {
  if (not input_helper::is_pos_number(std::to_string(loc))
      or loc > sizeof(board_vals)) {
    std::cout << "Invalid input please try again" << std::endl;
    return false;
  }
  else {
    board_vals[loc-1] = mark;
    return true;
  }
}