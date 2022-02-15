#include "poker_hand.h"
#include <sstream>
poker_hand::poker_hand(/* args */) : 
  hand(std::vector<card>()) {}

poker_hand::~poker_hand() {}

std::string poker_hand::to_string() const {
  std::stringstream ss;
  const char* separator = "";
  for (card elem : hand) {
    ss << separator << elem.value << elem.suit;
    separator = ",";
  }
  return ss.str();
}

void poker_hand::add_card(const card card) {
  hand.push_back(card);
}