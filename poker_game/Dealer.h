#pragma once
#include "Deck.cpp"
class Dealer
{
private:
  //Deck to wrap
  Deck deck;
  //Cards discarded for the case that there are 7 players and they
  //all discard their hands
  std::vector<Card> discardedCards;
public:
  //Constructor
  Dealer();
  //Destructor
  ~Dealer();
  //@Brief - Creates a new deck for in between rounds
  void makeNewDeck ();
  //@Brief - Deals a card from the deck
  //@Return card
  Card dealCard();
  //@Brief - Shuffles the Deck
  void Shuffle();
  //@Brief - Adds card to the discard pile
  //@Param[in] discarded - Vector of discarded cards to add
  void addToDiscardPile(const std::vector<Card> & discarded);
};
