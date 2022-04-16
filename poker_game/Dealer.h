#pragma once
#include "Deck.h"

class Dealer
{
private:
  /* data */
  Deck deck =  Deck();
public:
  Dealer() {}
  ~Dealer() {}

  void makeNewDeck () {
    deck = Deck();
  }

  Card dealCard() {
    Card ret = deck.draw();
    return ret;
  }

  void Shuffle() {
    deck.shuffle();
  }
};
