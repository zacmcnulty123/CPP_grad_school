#pragma once
#include "../poker_hands/card.h"
#include <time.h> 
#include <vector>
class Deck
{
private:
  //Vector to wrap
  std::vector<Card> deck;
public:
  //@Brief swap two elements
  //@Param[in] *a - pointer to the left element
  //@Param[in] *b - pointer to the right element
  template<typename T>
  static void swap (T *a, T *b) {
    T temp = *a;
    *a = *b;
    *b = temp;
  }
  //Constructor that makes a standard 52 card deck
  Deck() {
    int idx = 0;
    for (std::string suit : SUITS) {
      for (auto [key, value] : CARD_TO_VALUE_MAP) {
        deck.push_back(Card(key, suit));
      }
    }
  }
  //Destructor
  ~Deck() {};
  //@Brief - Shuffles the deck
  void shuffle() {
    std::srand(time(NULL));
    for (int i = deck.size() - 1; i > 0; i--) {
      int j =  std::rand() % (i + 1);
      swap(&deck[i], &deck[j]);
    }
  }
  //@Brief - Draws a card off the that "stack"
  //@return - Card
  Card draw() {
    Card ret = deck.back();
    deck.pop_back();
    return ret;
  }
  //@Brief - puts a card on the back
  //@Param[in] card - card to add to the deck
  void addCard(const Card & card) {
    deck.push_back(card);
  }
  //@Brief - returns the number of cards in the deck
  //@Return inr
  int getNumCardsLeftInDeck() const {
    return deck.size();
  }
};