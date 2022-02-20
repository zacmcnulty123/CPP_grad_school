#pragma once
#include "card.h"
#include <vector>
#include <iostream>
#include<map>

enum HandTypeE {eNotEnoughCards = 0,
                eHighCard = 1,
                eOnePair = 2,
                eTwoPair = 3,
                eThreeOfAKind = 4,
                eStraight = 5,
                eFlush = 6,
                eFullHouse = 7,
                eFourOfAKind = 8,
                eStraightFlush = 9};

struct Props {
  unsigned int threeOfKindVal = 0;
  std::array<unsigned, 2> twoPairVal;
  unsigned int onePairVal = 0;
  std::array<unsigned int, 2> fullHouseVal;
  unsigned int fourOfAKindVal = 0;
  std::vector<unsigned int> tieBreakers;
  bool isLowStraight = false;
};

class PokerHand {
  public:
    PokerHand(/* args */);

    std::string toString() const;

    //Add a card to the poker hand
    void addCard(const Card card);

    HandTypeE getHandType() const;

    std::vector<Card> getHand() const;

    int compare(const PokerHand & handToCompare) const;

    ~PokerHand();
  private:
    std::vector<Card> hand;
    HandTypeE handType;

    Props handProps;

    Props getProperties() const;

    void setHandType();
    bool isStraightFlush();

    bool isStraight();

    bool isFlush();
    
    bool isFullHouse();

    int countDuplicates() const;
    std::map<unsigned int, int> getDuplicateCountMap() const;
    bool isOneOrTwoPair();

    int handleTypeTieBreak(const PokerHand & comp, const HandTypeE handType) const;

    int handleTieBreakers(const std::vector<unsigned int> tieBreakers) const;
};

std::ostream &operator<<(std::ostream &out, const PokerHand hand) {
  out << hand.toString();
  return out;
}

std::ostream &operator<<(std::ostream &out, const HandTypeE type) {
  switch(type) {
    case eStraight: {
      out << "Straight" << std::endl;
      break;
    }
    case eStraightFlush: {
      out << "Straight Flush" << std::endl;
      break;
    }
    case eOnePair: {
      out << "One Pair" << std::endl;
      break;
    }
    case eTwoPair: {
      out << "Two Pair" << std::endl;
      break;
    }
    case eFourOfAKind: {
      out << "Four of a Kind" << std::endl;
      break;
    }
    case eHighCard: {
      out << "High Card" << std::endl;
      break;
    }
    case eFullHouse: {
      out << "Full House" << std::endl;
      break;
    }
    case eThreeOfAKind: {
      out << "Three of a Kind" << std::endl;
      break;
    }
    case eFlush: {
      out << "Flush" << std::endl;
      break;
    }
    default : {
      out << "Type doesn't exist" << std::endl;
    }
  }
  return out;
}