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
    ss << separator << hand[i].displayValue << hand[i].suit;
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

Props PokerHand::getProperties() const {
  return handProps;
}

int PokerHand::handleTieBreak(const PokerHand & comp, const HandTypeE handType) const {
  int ret = 0;
  int res = 0;
  std::vector<Card> compVals = comp.getHand();
  //TODO: Edge cases: Full House, Three of a kind, Four of a kind,
  //One pair
  switch (handType) {
    case eStraight:
    case eStraightFlush:
    case eHighCard:
    case eFlush:
      for (int i = hand.size()-1; i >= 0; i--) {
        if (hand[i] < compVals[i]) {
          //Found a hand that is the winner.
          //It is the second hand
          ret = 2;
          break;
        }
        else if (hand[i] == compVals[i]) {
          //Hands are still equal need to continue searching
          ret = 0;
          continue;
        }
        else {
          //Found a hand that is the winner
          //It is the first hand
          ret = 1;
          break;
        }
      }
      break;
    case eFourOfAKind: {
      Props compareProps = comp.getProperties();
      if (handProps.fourOfAKindVal == compareProps.fourOfAKindVal) {
        //Handle tiebreak
        if (handProps.tieBreakers[0] == compareProps.tieBreakers[0]) {
          break;
        }
        else if (handProps.tieBreakers[0] < compareProps.tieBreakers[0]) {
          ret = 2;
        }
        else {
          ret = 1;
        }
      }
      else if (handProps.fourOfAKindVal < compareProps.fourOfAKindVal) {
        ret = 2;
      }
      else {
        ret = 1;
      }
      break;
    }
    case eThreeOfAKind: {
      Props compareProps = comp.getProperties();
      if (handProps.threeOfKindVal == compareProps.threeOfKindVal) {
      //Handle tiebreak
        if (handProps.tieBreakers[1] == compareProps.tieBreakers[1]) {
          if (handProps.tieBreakers[0] == compareProps.tieBreakers[0]) {
            break;
          }
          else if (handProps.tieBreakers[0] < compareProps.tieBreakers[0]) {
            ret = 2;
          }
          else {
            ret = 1;
          }
        }
        else if (handProps.tieBreakers[1] < compareProps.tieBreakers[1]) {
          ret = 2;
        }
        else {
          ret = 1;
        }
      }
      else if (handProps.threeOfKindVal < compareProps.threeOfKindVal) {
        ret = 2;
      }
      else {
        ret = 1;
      }
      break;
    }
    case eTwoPair: {
      Props compareProps = comp.getProperties();
      if (handProps.twoPairVal[1] == compareProps.twoPairVal[1]) {
        //Handle tiebreak
        if (handProps.twoPairVal[0] == compareProps.twoPairVal[0]) {
          //Handle final tiebreakers
          if (handProps.tieBreakers[0] == compareProps.tieBreakers[0]) {
          break;
          }
          else if (handProps.tieBreakers[0] < compareProps.tieBreakers[0]) {
            ret = 2;
          }
          else {
            ret = 1;
          }
        }
        else if (handProps.twoPairVal[0] < compareProps.twoPairVal[0]) {
          ret = 2;
        }
        else {
          ret = 1;
        }
      }
      else if (handProps.twoPairVal[1] < compareProps.twoPairVal[1]) {
        ret = 2;
      }
      else {
        ret =1;
      }
      break;
    }
    
    case eOnePair: {
      Props compareProps = comp.getProperties();
      if (handProps.onePairVal == compareProps.onePairVal) {
        //Handle tiebreakers
        for (int i = handProps.tieBreakers.size()-1; i >=0; i--) {
          if (handProps.tieBreakers[i] == compareProps.tieBreakers[i]) {
            continue;
          }
          else if (handProps.tieBreakers[i] < compareProps.tieBreakers[i]) {
            ret = 2;
            break;
          }
          else {
            ret = 1;
            break;
          }
        }
      }
      else if (handProps.onePairVal < compareProps.onePairVal) {
        ret = 2;
      }
      else {
        ret = 1;
      }
      break;
    }
    case eFullHouse: {
      Props compareProps = comp.getProperties();
      if (handProps.fullHouseVal[0] == compareProps.fullHouseVal[0]) {
        if (handProps.fullHouseVal[1] == compareProps.fullHouseVal[1]) {
          break;
        }
        else if (handProps.fullHouseVal[1] < compareProps.fullHouseVal[1]) {
          ret = 2;
        }
        else {
          ret =1;
        }
      }
      else if (handProps.fullHouseVal[0] < compareProps.fullHouseVal[0]) {
        ret = 2;
      }
      else {
        ret = 1;
      }
    }
    default:
      break;
  }
  return ret;
}

int PokerHand::compare(const PokerHand & handToCompare) const {
  int ret = 1;
  if (handType < handToCompare.getHandType()) {
    ret = 2;
  }
  else if (handType == handToCompare.getHandType()) {
    ret = handleTieBreak(handToCompare,
                        handType);
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
    std::map<unsigned int, int> counter;
    for (int i = 0; i < hand.size(); i++) {
      counter[hand[i].value]++;
    }
    for (const auto &kv : counter) {
      //Full house is a 3 of a kind and a 2 of a kind
      //So is I find a duplicate of both 2 and 3 cards
      //Then this hand MUST be a full house
      if (kv.second == 4) {
        handProps.fourOfAKindVal = kv.first;
      }
      else {
        handProps.tieBreakers.push_back(kv.first);
      }
    }
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

bool PokerHand::isOneOrTwoPair() {
  bool isOnePair = true;
  std::map<unsigned int, int> counter;
  for (int i = 0; i < hand.size(); i++) {
    counter[hand[i].value]++;
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
  if (isOnePair) {
    for (const auto &kv : counter) {
      //2 means that there are multiple of the given value
      //Want to know how many pairs exist
      if (kv.second == 2) {
        handProps.onePairVal = kv.first;
      }
      else {
        handProps.tieBreakers.push_back(kv.first);
      }
    }
  }
  else {
    int i = 0;
    for (const auto &kv : counter) {
      //2 means that there are multiple of the given value
      //Want to know how many pairs exist
      if (kv.second == 2) {
        handProps.twoPairVal[i++] = kv.first;
      }
      else {
        handProps.tieBreakers.push_back(kv.first);
      }
    }
    std::sort(handProps.twoPairVal.begin(), handProps.twoPairVal.end());
  }
  std::sort(handProps.tieBreakers.begin(), handProps.tieBreakers.end());
  return isOnePair;
}

bool PokerHand::isFullHouse() {
  bool isFullHouse = true;
  std::map<unsigned int, int> counter;
  for (int i = 0; i < hand.size(); i++) {
    counter[hand[i].value]++;
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
  if (isFullHouse) {
    for (auto kv : counter) {
      if (kv.second == 3) {
        handProps.fullHouseVal[0] = kv.first;
      }
      else if (kv.second == 2) {
        handProps.fullHouseVal[1] = kv.first;
      }
    }
  }
  else {
    for (auto kv : counter) {
      if (kv.second == 3) {
        handProps.threeOfKindVal = kv.first;
      }
      else {
        handProps.tieBreakers.push_back(kv.first);
      }
    }
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