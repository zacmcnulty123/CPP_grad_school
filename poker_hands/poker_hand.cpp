#pragma once
#include "poker_hand.h"
#include <sstream>
#include <map>
#include <algorithm>
#include <numeric>
PokerHand::PokerHand(/* args */) : 
  hand(std::vector<Card>()),
  handType (eNotEnoughCards) {}

PokerHand::~PokerHand() {}

std::string PokerHand::toString() const {
  std::stringstream ss;
  const char* separator = "";
  for (Card card : hand) {
    ss << separator << card.displayValue << card.suit;
    separator = ",";
  }
  // ss << std::endl << handType;
  return ss.str();
}

void PokerHand::addCard(const Card card) {
  hand.push_back(card);
  if (hand.size() > 4) {
    //Make it easier to write downstream operations
    std::sort(hand.begin(), hand.end());
    setHandType();
  }
}

HandTypeE PokerHand::getHandType() const {
  return handType;
}

std::vector<Card> PokerHand::getHand() const {
  return hand;
}

void PokerHand::setHandType() {
  //This is a weird name since I'm looking for 
  //four of a kind, three of a kind ect with this call as well
  int dupeCount = countDuplicates();
  if (dupeCount == 0) {
    //Check for hands that involve singletons
    if (isStraightFlush()) {
    handType = eStraightFlush;
    }
    else if (isStraight()) {
      handType = eStraight;
    }
    else if (isFlush()) {
      handType = eFlush;
    }
    else {
      handType = eHighCard;
    }
  }
  else if (dupeCount == 2) {
    //Check for Two Pair and One Pair
    if (isOneOrTwoPair()) {
      handType = eOnePair;
    }
    else {
      handType = eTwoPair;
    }
  }
  else if (dupeCount == 3) {
    //Check for three of a kind and full house
    if (isFullHouse()) {
      handType = eFullHouse;
    }
    else {
      handType = eThreeOfAKind;
    }
  }
  else {
    //This means it is 4 of a kind
    handType = eFourOfAKind;
  }
}

bool PokerHand::isStraight() const {
  bool isStraight = true;
  for (int i = 1; i < hand.size(); i++) {
    //Want to check if the value sequence is contiguous
    //TODO: IF THE HAND CONTAINS AN ACE NEED TO CHECK IF
    // IT CAN BE COUNTED AS A LOW
    if (hand[i].value - hand[i-1].value > 1) {
      isStraight = false;
      break;
    }
  }
  return isStraight;
}

bool PokerHand::isStraightFlush() const {
  bool isStraightFlush = true;
  if (not isStraight() or not isFlush()) {
    isStraightFlush =  false;
  }
  return isStraightFlush;
}

bool PokerHand::isOneOrTwoPair() const {
  bool isOnePair = true;
  std::map<std::string, int> counter;
  for (int i = 0; i < hand.size(); i++) {
    counter[hand[i].displayValue]++;
  }
  unsigned int count = 0;
  for (const auto &kv : counter) {
    //2 means that there are multiple of the given value
    //Want to know how many pairs exist
    if (kv.second == 2) {
      count++;
    }
  }
  //If two pairs exists this is a twoPair hand
  //It is a one pair hand otherwise
  if (count == 2) {
    isOnePair = false;
  }
  return isOnePair;
}

bool PokerHand::isFullHouse() const {
  bool isFullHouse = true;
  std::map<std::string, int> counter;
  for (int i = 0; i < hand.size(); i++) {
    counter[hand[i].displayValue]++;
  }
  unsigned int count = 0;
  for (const auto &kv : counter) {
    //Full house is a 3 of a kind and a 2 of a kind
    //So is I find a duplicate of both 2 and 3 cards
    //Then this hand MUST be a full house
    if (kv.second == 3) {
      count++;
    }
    else if (kv.second == 2) {
      count++;
    }
  }
  if (count == 1) {
    isFullHouse = false;
  }
  return isFullHouse;
}

bool PokerHand::isFlush() const {
  bool isFlush = true;;
  std::string tempSuit = hand[0].suit;
  for (int i = 1; i < hand.size(); i++) {
    //std::string compare returns 1 if it is not
    // Equal so the logic is inverse
    if (tempSuit.compare(hand[i].suit)) {
      isFlush = false;
    }
  }
  return isFlush;
}

int PokerHand::countDuplicates() const {
  std::map<std::string, int> counter;
  for (int i = 0; i < hand.size(); i++) {
    counter[hand[i].displayValue]++;
  }
  if (counter.size() == hand.size()) {
    return 0;
  }
  auto tmp =
      std::max_element(counter.begin(), counter.end(),
            [](const std::pair<std::string, unsigned int> & x,
              const std::pair<std::string, unsigned int> & y)->bool 
              {return x.second < y.second;});
  return tmp->second;
}