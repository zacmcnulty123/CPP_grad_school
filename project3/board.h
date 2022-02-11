#pragma once
#include<iostream>
#include<string>
#include<sstream>
#include<vector>

//Assumes that the user is using a terminal
class board_drawer
{
  private:
    //char can hold numberic and symbolic values. Make the board
    //a simple array of chars
    // 0 is an invalid location but this helps resolves off by 1 errors bececause
    // the players will be using base 1
    char board_vals[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    //Define some variables to make the board drawer a bit simpler
    //to write.
    std::string title = "Tic Tac Toe\n";
    std::string section_break = "___|___|___\n";
    std::string vert_bars = "   |   |   \n";

  public:
    //Default constructor doesn't need to do anything
    //Want the calling program when to choose to redraw the game board
    board_drawer(/* args */);
    ~board_drawer();

    //@Brief - Draws the tic-tac-toe-board with
    // values that are stored in the board_vals member variable
    //@return void
    void redraw_board();

    //@Brief - Given a board location and symbol to mark the board with
    // put mark at the location
    //@Param[in] loc - Board location to put the mark on
    //@Param[in] mark - non-numeric symbol to place on the board
    void mark_board(int loc, char mark);

    //@Brief - Checks the gamestate to see if a player one or
    // if the game is at a stalemate
    // @Return int - 1 if a player 1, 0 if the game is at a stalemate
    // 0 otherwise
    int check_win();

    //@Brief - Returns a list of valid game moved left on the board
    //@Return std::vector<int> list of valid board locations
    std::vector<int> get_unused_board_positions();
};
