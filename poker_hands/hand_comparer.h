#pragma once
#include "poker_hand.cpp"
#include "card.h"
int handleTieBreak(const std::vector<Card> hand1, const
  std::vector<Card> hand2) {
  int res = 0;
  //TODO: Edge cases: Full House, Three of a kind, Four of a kind,
  //One pair
  for (int i = hand1.size()-1; i >= 0; i--) {
    if (hand1[i] < hand2[i]) {
      //Found a hand that is the winner.
      //It is the second hand
      res = 2;
      break;
    }
    else if (hand1[i] == hand2[i]) {
      //Hands are still equal need to continue searching
      res = 0;
      continue;
    }
    else {
      //Found a hand that is the winner
      //It is the first hand
      res = 1;
      break;
    }
  }
  return res;
}

//Compares two hands
// 0 = equal
// 1 = hand 1 greater
// 2 = hand 2 greater
int compareHands(const PokerHand & hand1, const PokerHand  & hand2) {
  int res = 1;
    if (hand1.getHandType() < hand2.getHandType()) {
      res = 2;
    }
    else if (hand1.getHandType() == hand2.getHandType()) {
      res = handleTieBreak(hand1.getHand(), hand2.getHand());
    }
    return res;
}