#pragma once
#include "card.h"
#include <vector>
#include <iostream>
class PokerHand
{
  public:
    PokerHand(/* args */);

    //Evaluate the value of the hand
    int evaluateHand() const;

    std::string toString() const;

    //Add a card to the poker hand
    void addCard(const Card card);

    
// TODO: Determine if this should be a const function
// TODO: Pick a better name
    int getHandValue();
    ~PokerHand();
  private:
    std::vector<Card> hand;


    bool isStraightFlush();

    bool isStraight();
};


bool operator>(const PokerHand hand1, const PokerHand hand2) {
  // implement this somewhere
  return true;
}

std::ostream &operator<<(std::ostream &out, const PokerHand hand) {
  out << hand.toString() << std::endl;
  return out;
}
