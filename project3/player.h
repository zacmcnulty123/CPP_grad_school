#pragma once
#include<string>
#include<ostream>
class player
{
private:
  unsigned int num_wins;
  std::string name;
  char mark;
public:
  player(std::string player_name , char player_mark);
  ~player();

  std::string to_string() const;

  int choose_random_number(std::vector<int> valid_pos);
};


std::ostream &operator<<(std::ostream &out, const player & player) {
  std::cout << player.to_string();
  return out;
}