#pragma once
#include <map>
#include <array>
class poker_hand
{
  private:
    const std::map<std::string, unsigned int> CARD_TO_VALUE_MAP = {
      std::make_pair("2", 2),
      std::make_pair("3", 3),
      std::make_pair("4", 4),
      std::make_pair("5", 5),
      std::make_pair("6", 6),
      std::make_pair("7", 7),
      std::make_pair("8", 8),
      std::make_pair("9", 9),
      std::make_pair("10", 10),
      std::make_pair("J", 11),
      std::make_pair("Q", 12),
      std::make_pair("K", 13),
      std::make_pair("A", 14)};

    const std::array<std::string, 4> SUITS = {
      "spades", "hearts", "diamonds", "clubs"};

  public:
    struct {
      unsigned int value;
      std::string suit; 
    } card;

    poker_hand(/* args */);
    ~poker_hand();
};