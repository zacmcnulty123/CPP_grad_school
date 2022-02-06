#pragma once
#include<iostream>
#include<string>
#include<sstream>
class board_drawer
{
  private:
    char board_vals[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    std::string title = "Tic Tac Toe\n";
    std::string section_break = "___|___|___\n";
    std::string vert_bars = "   |   |   \n";


    std::string board_vals_str[3];

  public:
    board_drawer(/* args */);
    ~board_drawer();

    void redraw_board();

    bool mark_board(int loc, char mark);
};
