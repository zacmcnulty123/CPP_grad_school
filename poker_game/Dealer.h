#pragma once
#include "Deck.h"
class Dealer
{
private:
  //Deck to wrap
  Deck deck = Deck();
  //Cards discarded for the case that there are 7 players and they
  //all discard their hands
  std::vector<Card> discardedCards = std::vector<Card>();
public:
  //Constructor
  Dealer() {
    deck.shuffle();
  }
  //Destructor
  ~Dealer() {}
  //@Brief - Creates a new deck for in between rounds
  void makeNewDeck () {
    deck = Deck();
    deck.shuffle();
  }
  //@Brief - Deals a card from the deck
  //@Return card
  Card dealCard() {
    Card ret = deck.draw();
    if (deck.getNumCardsLeftInDeck() == 0) {
      for (const Card & card : discardedCards) {
        deck.addCard(card);
      }
      deck.shuffle();
    }
    return ret;
  }
  //@Brief - Shuffles the Deck
  void Shuffle() {
    deck.shuffle();
  }
  //@Brief - Adds card to the discard pile
  //@Param[in] discarded - Vector of discarded cards to add
  void addToDiscardPile(const std::vector<Card> & discarded) {
    for (const Card & card : discarded) {
      discardedCards.push_back(card);
    }
  }
};
