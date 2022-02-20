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
  for (int i = hand.size()-1; i >= 0; i--) {
    ss << separator << hand[i];
    separator = ",";
  }
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

Props PokerHand::getProperties() const {
  return handProps;
}

int PokerHand::handleTieBreakers(const std::vector<unsigned int> tieBreakers) const {
  int ret = 0;
  for (int i = handProps.tieBreakers.size()-1; i >=0; i--) {
    if (handProps.tieBreakers[i] == tieBreakers[i]) {
      continue;
    }
    else {
      ret = (handProps.tieBreakers[i] < tieBreakers[i]) ? 2 : 1;
      break;
    }
  }
  return ret;
}

int PokerHand::handleTypeTieBreak(const PokerHand & comp, const HandTypeE handType) const {
  int ret = 0;
  int res = 0;
  Props compareProps = comp.getProperties();
  switch (handType) {
    case eStraight:
    case eStraightFlush: {
      if (handProps.isLowStraight and compareProps.isLowStraight) {
        //This means that both hands are a low straight and must be equal
        break;
      }
      else if (handProps.isLowStraight) {
        ret = 2;
        break;
      }
      else if (compareProps.isLowStraight) {
        ret = 1;
        break;
      }
      //Fall through otherwise
    }
    case eHighCard:
    case eFlush:
      ret = handleTieBreakers(compareProps.tieBreakers);
      break;
    case eFourOfAKind: {
      if (handProps.fourOfAKindVal == compareProps.fourOfAKindVal) {
        //Handle tiebreak
        ret = handleTieBreakers(compareProps.tieBreakers);
      }
      else {
        ret = (handProps.fourOfAKindVal < compareProps.fourOfAKindVal) ? 2 : 1;
      }
      break;
    }
    case eThreeOfAKind: {
      if (handProps.threeOfKindVal == compareProps.threeOfKindVal) {
        //Handle tiebreak
        ret = handleTieBreakers(compareProps.tieBreakers);
      }
      else {
        ret = (handProps.threeOfKindVal < compareProps.threeOfKindVal) ? 2 : 1;
      }
      break;
    }
    case eTwoPair: {
      if (handProps.twoPairVal[1] == compareProps.twoPairVal[1]) {
        //Handle tiebreak
        if (handProps.twoPairVal[0] == compareProps.twoPairVal[0]) {
          //Handle final tiebreakers
          ret = handleTieBreakers(compareProps.tieBreakers);
        }
        else { 
          ret = (handProps.twoPairVal[0] < compareProps.twoPairVal[0]) ? 2 : 1; 
        }
      }
      else {
        ret = (handProps.twoPairVal[1] < compareProps.twoPairVal[1]) ? 2 : 1;
      }
      break;
    }
    case eOnePair: {
      if (handProps.onePairVal == compareProps.onePairVal) {
        //Handle tiebreakers
        ret = handleTieBreakers(compareProps.tieBreakers);
      }
      else {
        ret = (handProps.onePairVal < compareProps.onePairVal) ? 2 : 1; 
      }
      break;
    }
    case eFullHouse: {
      if (handProps.fullHouseVal[0] == compareProps.fullHouseVal[0]) {
        if (handProps.fullHouseVal[1] == compareProps.fullHouseVal[1]) {
          break;
        }
        else {
          ret = (handProps.fullHouseVal[1] < compareProps.fullHouseVal[1]) ? 2 : 1;
        }
      }
      else {
        ret = (handProps.fullHouseVal[0] < compareProps.fullHouseVal[0]) ? 2 : 1; 
      }
    }
    default:
      break;
  }
  return ret;
}

int PokerHand::compare(const PokerHand & handToCompare) const {
  int ret = 0;
  if (handType == handToCompare.getHandType()) {
    ret = handleTypeTieBreak(handToCompare,
                            handType);
  }
  else {
    ret = (handType < handToCompare.getHandType()) ? 2 : 1;
  }
  return ret;
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
      //Need to reset tiebreakers since the 
      //Straight/flush test might have added some values in
      handProps.tieBreakers = std::vector<unsigned int>();
      handType = eHighCard;
      for (Card card : hand) {
        handProps.tieBreakers.push_back(card.getValue());
      }
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
    std::map<unsigned int, int> counter = getDuplicateCountMap();
    for (const auto &kv : counter) {
      if (kv.second == 4) {
        handProps.fourOfAKindVal = kv.first;
      }
      else {
        handProps.tieBreakers.push_back(kv.first);
      }
    }
  }
}

bool PokerHand::isStraight() {
  handProps.tieBreakers = std::vector<unsigned int>();
  bool isStraight = true;
  handProps.isLowStraight = true;
  if (hand[hand.size()-1].getValue() == 14 and hand[0].getValue() == 2) {
    //Need to handle the low straight
    handProps.tieBreakers.push_back(hand[hand.size()-1].getValue());
    for (int i = 0; i < hand.size()-2; i++) {
      if (hand[i].getValue() - hand[i-1].getValue() > 1) {
        isStraight = false;
        handProps.isLowStraight = false;
        break;
      }
      else {
        handProps.tieBreakers.push_back(hand[i].getValue());
      }
    }
  }
  else {
    handProps.isLowStraight = false;
    handProps.tieBreakers.push_back(hand[0].getValue());
    for (int i = 1; i < hand.size(); i++) {
      //Want to check if the value sequence is contiguous
      if (hand[i].getValue() - hand[i-1].getValue() > 1) {
        isStraight = false;
        break;
      }
      else {
        handProps.tieBreakers.push_back(hand[i].getValue());
      }
    }
  }
  std::sort(handProps.tieBreakers.begin(), handProps.tieBreakers.end());
  return isStraight;
}

bool PokerHand::isStraightFlush() {
  bool isStraightFlush = true;
  if (not isStraight() or not isFlush()) {
    isStraightFlush =  false;
  }
  return isStraightFlush;
}

bool PokerHand::isOneOrTwoPair() {
  bool isOnePair = true;
  std::map<unsigned int, int> counter = getDuplicateCountMap();
  unsigned int count = 0;
  for (const auto &kv : counter) {
    //2 means that there are multiple of the given value
    //Want to know how many pairs exist
    if (kv.second == 2) {
      handProps.onePairVal = kv.first;
      handProps.twoPairVal[count] = kv.first;
      count++;
    } else {
      handProps.tieBreakers.push_back(kv.first);
    }
  }
  //If two pairs exists this is a twoPair hand
  //It is a one pair hand otherwise
  if (count == 2) {
    isOnePair = false;
  }
  std::sort(handProps.twoPairVal.begin(), handProps.twoPairVal.end());
  std::sort(handProps.tieBreakers.begin(), handProps.tieBreakers.end());
  return isOnePair;
}

bool PokerHand::isFullHouse() {
  bool isFullHouse = true;
  std::map<unsigned int, int> counter = getDuplicateCountMap();
  unsigned int count = 0;
  for (const auto &kv : counter) {
    //Full house is a 3 of a kind and a 2 of a kind
    //So is I find a duplicate of both 2 and 3 cards
    //Then this hand MUST be a full house
    if (kv.second == 3) {
      handProps.fullHouseVal[0] = kv.first;
      handProps.threeOfKindVal = kv.first;
      count++;
    }
    else if (kv.second == 2) {
      handProps.fullHouseVal[1] = kv.first;
      count++;
    }
    else {
      handProps.tieBreakers.push_back(kv.first);
    }
  }
  if (count == 1) {
    isFullHouse = false;
  }
  std::sort(handProps.tieBreakers.begin(), handProps.tieBreakers.end());
  return isFullHouse;
}

bool PokerHand::isFlush() {
  handProps.tieBreakers = std::vector<unsigned int>();
  bool isFlush = true;;
  std::string tempSuit = hand[0].getSuit();
  for (int i = 0; i < hand.size(); i++) {
    //std::string compare returns 1 if it is not
    // Equal so the logic is inverse
    if (tempSuit.compare(hand[i].getSuit())) {
      isFlush = false;
    }
    handProps.tieBreakers.push_back(hand[i].getValue());
  }
  std::sort(handProps.tieBreakers.begin(), handProps.tieBreakers.end());
  return isFlush;
}

int PokerHand::countDuplicates() const {
  std::map<unsigned int, int> counter = getDuplicateCountMap();
  if (counter.size() == hand.size()) {
    return 0;
  }
  auto tmp =
      std::max_element(counter.begin(), counter.end(),
            [](const std::pair<unsigned int, unsigned int> & x,
              const std::pair<unsigned int, unsigned int> & y)->bool 
              {return x.second < y.second;});
  return tmp->second;
}

std::map<unsigned int, int> PokerHand::getDuplicateCountMap() const {
  std::map<unsigned int, int> counter;
  for (int i = 0; i < hand.size(); i++) {
    counter[hand[i].getValue()]++;
  }
  return counter;
}