#include "board.h"
#include "input_helper.h"

board_drawer::board_drawer() {}
board_drawer::~board_drawer() {}

void board_drawer::redraw_board() {
  //this clears the terminal output
  //This works for Unix or Windows Based OS
  system("cls||clear");
  std::cout << title << vert_bars
  << " " << board_vals[1] << " | " << board_vals[2] << " | " << board_vals[3]
  << std::endl <<  section_break << vert_bars
  << " " << board_vals[4] << " | " << board_vals[5] << " | " << board_vals[6]
  << std::endl <<  section_break << vert_bars
  << " " << board_vals[7] << " | " << board_vals[8] << " | " << board_vals[9]
  << std::endl <<  section_break << vert_bars;
}

void board_drawer::mark_board(int loc, char mark) {
  if (loc > sizeof(board_vals)) {
    //Want to ensure that the input is a valid board location
    throw std::invalid_argument("Received invalid board location");
  }
  else if (isdigit(mark)) {
    //Do not want numeric mark values
    throw std::invalid_argument("Mark must be non-numberic");
  }
  else {
    //Mark the board
    board_vals[loc] = mark;
  }
}

int board_drawer::check_win() {
  //Check all win conditions
  if ((board_vals[1] == board_vals[2] and board_vals[2] == board_vals[3])
    or (board_vals[4] == board_vals[5] and board_vals[5] == board_vals[6])
    or (board_vals[7] == board_vals[8] and board_vals[8] == board_vals[9])
    or (board_vals[1] == board_vals[5] and board_vals[5] == board_vals[9])
    or (board_vals[3] == board_vals[5] and board_vals[5] == board_vals[7])
    or (board_vals[1] == board_vals[4] and board_vals[4] == board_vals[7])
    or (board_vals[2] == board_vals[5] and board_vals[5] == board_vals[8])
    or (board_vals[3] == board_vals[6] and board_vals[6] == board_vals[9])) {
      return 1;
  }
  //Check if there are any valid moves left.
  else if (board_vals[1] != '1' and board_vals[2] != '2' and board_vals[3] != '3' 
    and board_vals[4] != '4' and board_vals[5] != '5' and board_vals[6] != '6' 
    and board_vals[7] != '7' and board_vals[8] != '8' and board_vals[9] != '9') {
      return 0;
    }
  else {
    return -1;
  }
}

std::vector<int> board_drawer::get_unused_board_positions() {
  std::vector<int> ret = std::vector<int>();
  for (int i = 1; i < sizeof(board_vals); i++) {
    std::string s;
    s.assign(1, board_vals[i]);
    //Since the board doesn't allow non-numeric symbols
    // Check the array positions of numeric values and return those
    // to the caller
    if (input_helper::is_pos_number(s)) {
      ret.push_back(i);
    }
  }
  return ret;
}

void board_drawer::board_reset() {
  char vals[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
  //Can't do direct assignment with basic arrays
  for (unsigned int i = 0; i < sizeof(board_vals); i++) {
    //Reset all of the values to the initialized values.
    board_vals[i] = vals[i];
  }
}