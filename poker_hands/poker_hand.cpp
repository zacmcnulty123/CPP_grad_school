#pragma once
#include"poker_hand.h"
#include<sstream>
#include<algorithm>
#include<numeric>
PokerHand::PokerHand(/* args */) : 
  hand(std::vector<Card>()),
  handType (eNotEnoughCards) {}

PokerHand::~PokerHand() {}

std::string PokerHand::toString() const {
  std::stringstream ss;
  const char* separator = "";
  //Hand is sorted each time a card is added.
  //It is easier to read L -> R when ordered
  //highest to lowest value.
  for (int i = hand.size()-1; i >= 0; i--) {
    ss << separator << hand[i];
    separator = ",";
  }
  return ss.str();
}

void PokerHand::addCard(const Card card) {
  //Will remove this restriction when a class algorithm
  //Is created to choose the BEST hand out of all possible
  //hands with 6 or greater cards.
  if (hand.size() >= 5) {
    throw std::invalid_argument("This hand cannot have anymore cards");
  }
  try {
    //Add the card
    hand.push_back(card);
    //Sort the hand; Low runtime inpact since hand sizes are small
    std::sort(hand.begin(), hand.end());
    //Once a hand has 5 cards we can properly evaluate it.
    if (hand.size() > 4) {
      setHandProperties();
    }
  }
  catch(std::invalid_argument e) {
    //Propagate exception upward
    throw std::invalid_argument(e.what());
  }
}

HandTypeE PokerHand::getHandType() const {
  return handType;
}

Props PokerHand::getProperties() const {
  return handProps;
}

int PokerHand::handleTieBreakers(const std::vector<unsigned int> tieBreakers) const {
  //Hands are equal until proven otherwise
  int ret = 0;
  //Walk through the tiebreakers from highest to lowest value
  for (int i = handProps.tieBreakers.size()-1; i >=0; i--) {
    if (handProps.tieBreakers[i] == tieBreakers[i]) {
      //if equal keep searching
      continue;
    }
    else {
      //If our card is less; the other hand is great. Return 2. Otherwise
      // our hard is greater and we should return 1; 
      ret = (handProps.tieBreakers[i] < tieBreakers[i]) ? 2 : 1;
      break;
    }
  }
  return ret;
}

int PokerHand::handleTypeTieBreak(const PokerHand & comp) const {
  //Hands are equal until proven otherwise
  int ret = 0;
  //Get the list of properties of the hand we'd like to compare against
  Props compareProps = comp.getProperties();
  switch (handType) {
    case eStraight:
    case eStraightFlush: {
      //Handle the low straight case
      if (handProps.isLowStraight and compareProps.isLowStraight) {
        //This means that both hands are a low straight and must be equal
        break;
      }
      else if (handProps.isLowStraight) {
        //If we have a low straight our hand is automatically worse
        // than any other straight
        ret = 2;
        break;
      }
      else if (compareProps.isLowStraight) {
        //If the hand we compare to is a low straight we are automatically greater
        ret = 1;
        break;
      }
      //Fall through otherwise
    }
    case eHighCard:
    case eFlush:
      //Tie breaks handled via highest ranked card
      ret = handleTieBreakers(compareProps.tieBreakers);
      break;
    case eFourOfAKind: {
      //If the 4ok card is equal we need to handle the tiebreak
      if (handProps.fourOfAKindVal == compareProps.fourOfAKindVal) {
        //Handle tiebreak. Done by comparing the highest kind in the tiebreakers
        //In the case of 4ok it's the single kicker.
        ret = handleTieBreakers(compareProps.tieBreakers);
      }
      else {
        //If our 4ok value is less our hand is worse; set 2. Otherwise set 1;
        ret = (handProps.fourOfAKindVal < compareProps.fourOfAKindVal) ? 2 : 1;
      }
      break;
    }
    case eThreeOfAKind: {
      //If our 3ok card is equal handle the tiebreak
      if (handProps.threeOfKindVal == compareProps.threeOfKindVal) {
        //Handle tiebreak. Done by comparing the tiebreak cards. Take the
        //hand that has the highest
        ret = handleTieBreakers(compareProps.tieBreakers);
      }
      else {
        //If our 3ok value is less our hand is worse; set 2. Otherwise set 1;
        ret = (handProps.threeOfKindVal < compareProps.threeOfKindVal) ? 2 : 1;
      }
      break;
    }
    case eTwoPair: {
      //Compare the highest of the two pairs, if equal check the
      //lower pairs
      if (handProps.twoPairVal[1] == compareProps.twoPairVal[1]) {
        //Handle tiebreak; If the lower pairs match; Go to the tiebreak card
        if (handProps.twoPairVal[0] == compareProps.twoPairVal[0]) {
          //Handle final tiebreakers
          ret = handleTieBreakers(compareProps.tieBreakers);
        }
        else { 
          //If our lower pair is less; the other hand is greater; Return 2; 
          //Otherwise 1;
          ret = (handProps.twoPairVal[0] < compareProps.twoPairVal[0]) ? 2 : 1; 
        }
      }
      else {
        //If our highest pair is less; the other hand is greater; Return 2; 
        //Otherwise 1;
        ret = (handProps.twoPairVal[1] < compareProps.twoPairVal[1]) ? 2 : 1;
      }
      break;
    }
    case eOnePair: {
      //If the pairs are the same; handle tiebreakers
      if (handProps.onePairVal == compareProps.onePairVal) {
        //Handle tiebreakers
        ret = handleTieBreakers(compareProps.tieBreakers);
      }
      else {
        //If our pair is less; the other hand is greater; Return 2; Otherwise 1
        ret = (handProps.onePairVal < compareProps.onePairVal) ? 2 : 1; 
      }
      break;
    }
    case eFullHouse: {
      //Compare the 3s in the fullhouse; If equal go to the 2s
      if (handProps.fullHouseVal[0] == compareProps.fullHouseVal[0]) {
        //If the 2s are equal; The hands are equal
        if (handProps.fullHouseVal[1] == compareProps.fullHouseVal[1]) {
          break;
        }
        else {
          //If our 2s is less; The other hand is greater; return 2; otherwise 1
          ret =
            (handProps.fullHouseVal[1] < compareProps.fullHouseVal[1]) ? 2 : 1;
        }
      }
      else {
        //If our 3s is less; The other hand is greater; return 2; otherwise 1
        ret =
          (handProps.fullHouseVal[0] < compareProps.fullHouseVal[0]) ? 2 : 1; 
      }
    }
    default:
      //Do Nothing (Shouldn't ever reach here)
      break;
  }
  //Return the results
  return ret;
}

int PokerHand::compare(const PokerHand & handToCompare) const {
  //Need to have enough cards in both hands to compare.
  if (handType == HandTypeE::eNotEnoughCards
    or handToCompare.getHandType() == HandTypeE::eNotEnoughCards) {
      throw std::invalid_argument("Ones of the hands do not"
        "have enough cards to compare");
  }
  //NOTE from Zac: Wasn't sure if a hand with enough cards to compare
  //Would be inherently greater than one without enough cards.

  //Hands are equal until proven otherwise
  int ret = 0;
  //Compare the hand types first. Some hands are inherently greater than
  // others. If they are the same hand type need to handle the tiebreak
  if (handType == handToCompare.getHandType()) {
    ret = handleTypeTieBreak(handToCompare);
  }
  else {
    //If our handType is worse the other hand is greater; Return 2; Otherwise 1
    ret = (handType < handToCompare.getHandType()) ? 2 : 1;
  }
  //return the results
  return ret;
}

void PokerHand::setHandProperties() {
  //Get the number of cards that contain
  //The same value in the hand.
  //This helps determine which handTypes to 
  //check for
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
      //Hand must be highCard.
      handType = eHighCard;
      for (Card card : hand) {
        handProps.tieBreakers.push_back(card.getValue());
      }
    }
  }
  else if (dupeCount == 2) {
    //If the max number of duplicates in a hand is 2
    //then it is either a two pair or a one pair
    if (isOnePair()) {
      handType = eOnePair;
    }
    else {
      handType = eTwoPair;
    }
  }
  else if (dupeCount == 3) {
    //If the max number of duplicates in a hand is 3
    //then it is either a fullhouse or a 3ok
    if (isFullHouse()) {
      handType = eFullHouse;
    }
    else {
      handType = eThreeOfAKind;
    }
  }
  else {
    //Can only have a max of 4 duplicates. Which means this is 
    // a 4ok hand
    handType = eFourOfAKind;
    //Set hand properties
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
  //Reset tiebreakers so that we can fill them out
  handProps.tieBreakers = std::vector<unsigned int>();
  //The hand is a straight until proven otherwise
  bool isStraight = true;
  //The hand is a lowStraight until proven otherwise
  handProps.isLowStraight = true;
  //An Ace is a 14 and we can only have a lowStraight is AT LEAST
  // the hand also contains a 2.
  if (hand[hand.size()-1].getValue() == 14 and hand[0].getValue() == 2) {
    //Need to handle the low straight
    handProps.tieBreakers.push_back(hand[hand.size()-1].getValue());
    //Check if the hand is a straight
    for (int i = 0; i < hand.size()-2; i++) {
      if (hand[i].getValue() - hand[i-1].getValue() > 1) {
        isStraight = false;
        handProps.isLowStraight = false;
        break;
      }
      else {
        //Load up the tiebreaks
        handProps.tieBreakers.push_back(hand[i].getValue());
      }
    }
  }
  else {
    //Check to see is this is a normal straight
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
  //Sort the tiebreakers
  std::sort(handProps.tieBreakers.begin(), handProps.tieBreakers.end());
  return isStraight;
}

bool PokerHand::isStraightFlush() {
  //The hand is a straight flush until proben otherwise
  bool isStraightFlush = true;
  //Hand must be a straight and a flush to be a 
  // straight flush
  if (not isStraight() or not isFlush()) {
    isStraightFlush =  false;
  }
  return isStraightFlush;
}

bool PokerHand::isOnePair() {
  //Hand is a onePair until proven otherwise
  bool isOnePair = true;
  //Get the cards that have duplicates
  std::map<unsigned int, int> counter = getDuplicateCountMap();
  //Counter to determine if there are 1 or 2 pairs in the hand
  unsigned int count = 0;
  for (const auto &kv : counter) {
    //2 means that there are multiple of the given value
    //Want to know how many pairs exist
    if (kv.second == 2) {
      //Set the hand properties
      handProps.onePairVal = kv.first;
      handProps.twoPairVal[count] = kv.first;
      //Found a pair. Add it to the count!
      count++;
    }
    else {
      //Put in the tiebreakers
      handProps.tieBreakers.push_back(kv.first);
    }
  }
  //If two pairs exists this is a twoPair hand
  //It is a one pair hand otherwise
  if (count == 2) {
    isOnePair = false;
  }
  //Sort the cards for easier downstream comparison
  std::sort(handProps.twoPairVal.begin(), handProps.twoPairVal.end());
  std::sort(handProps.tieBreakers.begin(), handProps.tieBreakers.end());
  return isOnePair;
}

bool PokerHand::isFullHouse() {
  //Hand is a fullHouse until proven otherwise
  bool isFullHouse = true;
  std::map<unsigned int, int> counter = getDuplicateCountMap();
  unsigned int count = 0;
  for (const auto &kv : counter) {
    //Full house is a 3 of a kind and a 2 of a kind
    //So is I find a duplicate of both 2 and 3 cards
    //Then this hand MUST be a full house
    if (kv.second == 3) {
      //Fill out hand properties
      handProps.fullHouseVal[0] = kv.first;
      handProps.threeOfKindVal = kv.first;
      count++;
    }
    else if (kv.second == 2) {
      handProps.fullHouseVal[1] = kv.first;
      count++;
    }
    else {
      //Put in the tieBreakers
      handProps.tieBreakers.push_back(kv.first);
    }
  }
  if (count == 1) {
    isFullHouse = false;
  }
  //Sort the tiebreakers for easier downstream comparison
  std::sort(handProps.tieBreakers.begin(), handProps.tieBreakers.end());
  return isFullHouse;
}

bool PokerHand::isFlush() {
  handProps.tieBreakers = std::vector<unsigned int>();
  //Hand is a flush until proven otherwise
  bool isFlush = true;
  //All suits must match take a random card
  std::string tempSuit = hand[0].getSuit();
  for (int i = 0; i < hand.size(); i++) {
    //std::string compare returns 1 if it is not
    // Equal so the logic is inverse
    if (tempSuit.compare(hand[i].getSuit())) {
      isFlush = false;
    }
    //Put in the tie breakers
    handProps.tieBreakers.push_back(hand[i].getValue());
  }
  std::sort(handProps.tieBreakers.begin(), handProps.tieBreakers.end());
  return isFlush;
}

int PokerHand::countDuplicates() const {
  //Get a map the has all the duplicates and how many there are
  std::map<unsigned int, int> counter = getDuplicateCountMap();
  //this means that each value appears once. No need to find
  //Maximum
  if (counter.size() == hand.size()) {
    return 0;
  }
  //find the maximum in the map
  auto tmp =
      std::max_element(counter.begin(), counter.end(),
            [](const std::pair<unsigned int, unsigned int> & x,
              const std::pair<unsigned int, unsigned int> & y)->bool 
              {return x.second < y.second;});
  //Return the maximum in the map
  return tmp->second;
}

std::map<unsigned int, int> PokerHand::getDuplicateCountMap() const {
  //Count the duplicates
  std::map<unsigned int, int> counter;
  for (int i = 0; i < hand.size(); i++) {
    counter[hand[i].getValue()]++;
  }
  return counter;
}