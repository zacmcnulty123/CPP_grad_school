#pragma once
#include "Deck.h"

class Dealer
{
private:
  /* data */
  Deck deck =  Deck();
  std::vector<Card> discardedCards = std::vector<Card>();
public:
  Dealer() {
    deck.shuffle();
  }
  ~Dealer() {}

  void makeNewDeck () {
    deck = Deck();
    deck.shuffle();
  }

  Card dealCard() {
    Card ret = deck.draw();
    return ret;
  }

  void Shuffle() {
    deck.shuffle();
  }

  void addToDiscardPile(const std::vector<Card> & discarded) {
    for (const Card & card : discarded) {
      discardedCards.push_back(card);
    }
  }
};
