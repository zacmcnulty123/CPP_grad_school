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
  Deck();
  //Destructor
  ~Deck();
  //@Brief - Shuffles the deck
  void shuffle();
  //@Brief - Draws a card off the that "stack"
  //@return - Card
  Card draw() {
    Card ret = deck.back();
    deck.pop_back();
    return ret;
  }
  //@Brief - puts a card on the back
  //@Param[in] card - card to add to the deck
  void addCard(const Card & card);
  //@Brief - returns the number of cards in the deck
  //@Return inr
  int getNumCardsLeftInDeck() const;
};