#include "board.h"
#include "input_helper.h"
board_drawer::board_drawer() {

}
board_drawer::~board_drawer() {}
void board_drawer::redraw_board() {
  std::cout << "\033[2J\033[1;1H";
  std::cout << title << vert_bars
  << " " << board_vals[0] << " | " << board_vals[1] << " | " << board_vals[2]
  << std::endl <<  section_break << vert_bars
  << " " << board_vals[3] << " | " << board_vals[4] << " | " << board_vals[5]
  << std::endl <<  section_break << vert_bars
  << " " << board_vals[6] << " | " << board_vals[7] << " | " << board_vals[8]
  << std::endl <<  section_break << vert_bars;
}

bool board_drawer::mark_board(int loc, char mark) {
  if (loc > sizeof(board_vals)) {
    std::cout << "Invalid input please try again" << std::endl;
    return false;
  }
  else {
    board_vals[loc-1] = mark;
    return true;
  }
}

int board_drawer::check_win() {
  if ((board_vals[0] == board_vals[1] and board_vals[1] == board_vals[2])
    or (board_vals[3] == board_vals[4] and board_vals[4] == board_vals[5])
    or (board_vals[6] == board_vals[7] and board_vals[7] == board_vals[8])
    or (board_vals[0] == board_vals[4] and board_vals[4] == board_vals[8])
    or (board_vals[2] == board_vals[4] and board_vals[1] == board_vals[6])
    or (board_vals[0] == board_vals[3] and board_vals[3] == board_vals[6])
    or (board_vals[1] == board_vals[4] and board_vals[4] == board_vals[7])
    or (board_vals[2] == board_vals[5] and board_vals[5] == board_vals[8])) {
      return true;
    }
  else if (board_vals[0] != '1' and board_vals[1] != '2' and board_vals[2] != '3' 
    and board_vals[3] != '4' and board_vals[4] != '5' and board_vals[5] != '6' 
    and board_vals[7] != '7' and board_vals[7] != '8' and board_vals[8] != '9') {
      return 0;
    }
  else {
    return -1;
  }
}

std::vector<int> board_drawer::get_unused_board_positions() {
  std::vector<int> ret = std::vector<int>();
  for (int i = 0; i < sizeof(board_vals); i++) {
    std::string s;
    s.assign(1, board_vals[i]);
    if (input_helper::is_pos_number(s)) {
      ret.push_back(i);
    }
  }
  return ret;
}