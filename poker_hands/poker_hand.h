#pragma once
#include "card.h"
#include <vector>
class poker_hand
{
  public:
    poker_hand(/* args */);

    //Evaluate the value of the hand
    int evaluate_hand() const;

    //Add a card to the poker hand
    void add_card(const card);
    ~poker_hand();
  private:
    std::vector<card> hand;
};


bool operator>(const poker_hand hand1, const poker_hand hand2) {
  // implement this somewhere
  return true;
}