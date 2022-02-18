#pragma once
#include "card.h"
#include <vector>
#include <iostream>
#include<map>
class PokerHand
{
  public:
    PokerHand(/* args */);

    //Evaluate the value of the hand
    int evaluateHand() const;

    std::string toString() const;

    //Add a card to the poker hand
    void addCard(const Card card);

    void getHandType();
    ~PokerHand();
  private:
    std::vector<Card> hand;
    HandTypeE handType;

    bool isStraightFlush() const;

    bool isStraight() const;

    bool isFlush() const;
    
    int isFullHouse() const;

    int countDuplicates() const;

    int isOneOrTwoPair() const;
};

bool operator>(const PokerHand hand1, const PokerHand hand2) {
  // implement this somewhere
  return true;
}

std::ostream &operator<<(std::ostream &out, const PokerHand hand) {
  out << hand.toString() << std::endl;
  return out;
}