#pragma once
#include "../poker_hands/card.h"
#include <time.h> 
#include <vector>
class Deck
{
private:
  /* data */
  std::vector<Card> deck;

  template<typename T>
  void swap (T *a, T *b) {
    T temp = *a;
    *a = *b;
    *b = temp;
  }
public:
  
  Deck() {
    int idx = 0;
    for (std::string suit : SUITS) {
      for (auto [key, value] : CARD_TO_VALUE_MAP) {
        deck.push_back(Card(key, suit));
      }
    }
  }
  ~Deck() {};

  void shuffle() {
    std::srand(time(NULL));

    for (int i = deck.size() - 1; i > 0; i--) {
      int j =  std::rand() % (i + 1);

      swap(&deck[i], &deck[j]);
    }
  }

  Card draw() {
    Card ret = deck.back();
    deck.pop_back();
    return ret;
  }

  std::string toString() {
    std::stringstream ss;

    ss << "Deck\n";
    for (Card card : deck) {
      ss << card << std::endl;
    }
    return ss.str();
  }
};

std::ostream &operator << (std::ostream &out, Deck deck) {
  out << deck.toString();
  return out;
}