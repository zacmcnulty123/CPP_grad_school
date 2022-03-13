#include "board.cpp"
#include "player.cpp"
#include "input_helper.h"
#include <chrono>
#include <thread>
using namespace std;
int main (void) {
  //Make the game board
  board_drawer board = board_drawer();
  //Allow the game to go on forever
  bool game_active = true;
  //Keep track of the player's turn
  int counter = 0;
  //Prompt human player for their name
  string input;
  cout << "Enter Player 1's name: ";
  cin >> input;
  //Create the players. Human is always x
  player players[2] = {player(input, 'x'),  player("Player 2", 'o')};
  while (game_active) {
    board.redraw_board();
    //Human players turn
    if (counter % 2 == 0) {
      bool invalid_input = true;
      int mark = 0;
      //Want to make sure that the human input is valid
      while (invalid_input) {
        cout << players[0] << " please choose a position to mark" << endl;
        cin >> mark;
        cout << endl;
        try {
          //Make sure that position to mark is valid
          if (input_helper::is_pos_number(std::to_string(mark)))
          {
            //Make sure that the move is possible on board
            std::vector<int> valid_pos = board.get_unused_board_positions();
            if (not std::count(valid_pos.begin(), valid_pos.end(), mark)) {
              cout << "Invalid input! please choose a valid position on the board!\n";
              continue;
            }
            //Note that mark board throws an exception if the input is invalid.
            //However, we only get to this point if mark is a valid game action.
            //Chose not to catch the exception because we will never hit it in
            //this function
            //Mark the board
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
    }
    else { //Computer player's turn
      //Don't want to update the board immediately with computer choice
      //Give human player a bit of time to process.
      this_thread::sleep_for(chrono::milliseconds(500));
      //Allow the computer player to pick a random valid board position
      int mark = players[1].choose_random_game_action(board.get_unused_board_positions());
      //Shouldn't ever trigger the catch but in case something goes really wrong...
      try {
        //Mark the board
        board.mark_board(mark, 'o');
      } //Catche the exception that mark_board can throw
      catch(std::invalid_argument &e) {
        cout << e.what() << endl;
      }
    }
    if (board.check_win() == 1) {
      board.redraw_board();
      //print the name of the current player who won
      cout << players[counter % 2] << " Won!" << endl;
      cout << "Would you like to play again? press y" << endl;
      std::string input;
      cin >> input;
      input_helper::to_lower(input);
      //if the input it y start a new game. Kill the program otherwise
      if (not input.compare("y")) {
        //reset the gamestate
        board.board_reset();
        counter = 0;
      }
      else {
        //End the game
        game_active = false;
      }
    }
    else if (board.check_win() == 0) {
      board.redraw_board();
      cout << "no winner could be determined!" << endl;
      cout << "Would you like to play again? press y" << endl;
      std::string input;
      cin >> input;
      input_helper::to_lower(input);
      //if the input it y start a new game. Kill the program otherwise
      if (not input.compare("y")) {
        board.board_reset();
        //reset the counter and start the game over again
        counter = 0;
      }
      else {
        //end the game
        game_active = false;
      }
    }
    else {
      //swap player's turn
      counter++;
    }
  }
  return 0;
};