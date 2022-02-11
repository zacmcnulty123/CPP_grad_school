#include "player.h"
player::player (std::string player_name, char player_mark) :
  num_wins(0) {
    name = player_name;
    mark = player_mark;
  }

player::~player() {}

std::string player::to_string() const {
  return name;
}

//Intended to be an overloadable function name to allow the player
//class to choose random actions for different games
int player::choose_random_game_action(std::vector<int> valid_pos) {
  //srand makes choosing random numbers more 'random'
  //rand will return the same sequence of numbers and only change
  // on recompile
  srand((unsigned int)time(NULL));
  int rand = std::rand() % valid_pos.size();
  return valid_pos[rand];
}