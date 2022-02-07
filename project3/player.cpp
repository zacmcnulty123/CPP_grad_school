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

int player::choose_random_number(std::vector<int> valid_pos) {
  srand((unsigned int)time(NULL));
  int rand = std::rand() % valid_pos.size();
  return valid_pos[rand] + 1;
}