#pragma once
#include<string>
#include<ostream>
class player
{
private:
  //Allow the player to be named
  std::string name;
  //character that the player will mark the game board with
  char mark;
public:
  //@Param[in] player_name - name for the given player
  //@Param[in] player_mark - mark that the player will use to
  // mark the game board
  player(std::string player_name , char player_mark);
  ~player();

  //@Brief - Returns the player name
  std::string to_string() const;

  //@Brief - Player chooses a random game action
  //@Param[in] valid_pos: valid game positions to choose from
  //@Return int - chosen random action
  int choose_random_game_action(std::vector<int> valid_pos);
};

std::ostream &operator<<(std::ostream &out, const player & player) {
  std::cout << player.to_string();
  return out;
}