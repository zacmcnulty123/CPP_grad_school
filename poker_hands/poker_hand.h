#pragma once
#include "card.h"
#include <vector>
#include <iostream>
#include<map>
class PokerHand
{
  public:
    PokerHand(/* args */);

    std::string toString() const;

    //Add a card to the poker hand
    void addCard(const Card card);

    //Want to get the best hand out the the list of hands
    //Might want to return a ranked list of hands
    std::string compareHands(const std::vector<PokerHand> hands) const;

    HandTypeE getHandType() const;

    std::vector<Card> getHand() const;

    ~PokerHand();
  private:
    std::vector<Card> hand;
    HandTypeE handType;

    void setHandType();
    bool isStraightFlush() const;

    bool isStraight() const;

    bool isFlush() const;
    
    bool isFullHouse() const;

    int countDuplicates() const;

    bool isOneOrTwoPair() const;

    int handleTieBreak(const PokerHand hand1, const PokerHand hand2) const;
};

// bool operator<(const PokerHand hand1, const PokerHand hand2) {
//   return compareHands(hand1, hand2);
// }

std::ostream &operator<<(std::ostream &out, const PokerHand hand) {
  out << hand.toString();
  return out;
}