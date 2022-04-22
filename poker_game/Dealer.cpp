#include "Dealer.h"
Dealer::Dealer() {
  deck = Deck();
  discardedCards = std::vector<Card>();
  deck.shuffle();
}
Dealer::~Dealer() {}
void Dealer::makeNewDeck() {
  deck = Deck();
  deck.shuffle();
}
Card Dealer::dealCard() {
  Card ret = deck.draw();
  if (deck.getNumCardsLeftInDeck() == 0) {
    for (const Card & card : discardedCards) {
      deck.addCard(card);
    }
    deck.shuffle();
  }
  return ret;
}
void Dealer::Shuffle() {
  deck.shuffle();
}
void Dealer::addToDiscardPile(const std::vector<Card> & discarded) {
  for (const Card & card : discarded) {
    discardedCards.push_back(card);
  }
}