#include "poker_hand.h"
#include <sstream>
PokerHand::PokerHand(/* args */) : 
  hand(std::vector<Card>()) {}

PokerHand::~PokerHand() {}

std::string PokerHand::toString() const {
  std::stringstream ss;
  const char* separator = "";
  for (Card card : hand) {
    ss << separator << card.displayValue << card.suit;
    separator = ",";
  }
  return ss.str();
}

void PokerHand::addCard(const Card card) {
  hand.push_back(card);
}

int PokerHand::getHandValue() {
  handTypeE handtype = eStraight;
  if (isStraightFlush()) {
    handtype = eStraightFlush;
  }
  else if (isStraight()) {
    handtype = eStraight;
  }
  std::cout << handtype;
  return 0;
}

bool PokerHand::isStraight() {
  std::vector<Card> tempHand = hand;
  std::sort(tempHand.begin(), tempHand.end());
  for (int i = 1; i < tempHand.size(); i++) {
    //Want to check if the value sequence is contiguous
    if (tempHand[i].value - tempHand[i].value > 1) {
      return false;
    }
  }
  return true;
}

bool PokerHand::isStraightFlush() {
  if (not isStraight()) {
    return false;
  }
  else {
    std::string tempSuit = hand[0].suit;
    for (int i = 1; i < hand.size(); i++) {
      //std::string compare returns 1 if it is not
      // Equal so the logic is inverse
      if (tempSuit.compare(hand[i].suit)) {
        return false;
      }
      else {
        return true;
      }
    }
  }
}