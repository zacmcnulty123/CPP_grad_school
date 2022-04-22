#include "Deck.h"
Deck::Deck() {
  int idx = 0;
  for (std::string suit : SUITS) {
    for (auto [key, value] : CARD_TO_VALUE_MAP) {
      deck.push_back(Card(key, suit));
    }
  }
}
Deck::~Deck() {}
void Deck::shuffle() {
  std::srand(time(NULL));
  for (int i = deck.size() - 1; i > 0; i--) {
    int j =  std::rand() % (i + 1);
    swap(&deck[i], &deck[j]);
  }
}
void Deck::addCard(const Card & card) {
  deck.push_back(card);
}
int Deck::getNumCardsLeftInDeck() const {
  return deck.size();
}