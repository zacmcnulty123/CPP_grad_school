#pragma once
#include"card.h"
#include<vector>
#include<iostream>
#include<map>

// Explicitly defined enum that defines
// The type of hand being worked with.
// Comes in handy to ranking hands via
// Type
// Ordered like to for keeping track of which hand types
// are greater than others.
enum class HandTypeE {eNotEnoughCards = 0,
                eHighCard = 1,
                eOnePair = 2,
                eTwoPair = 3,
                eThreeOfAKind = 4,
                eStraight = 5,
                eFlush = 6,
                eFullHouse = 7,
                eFourOfAKind = 8,
                eStraightFlush = 9};

class PokerHand {
  public:
    //Constructor
    PokerHand(/* args */);

    PokerHand(
      const PokerHand & cp) :
      hand(cp.hand),
      handType(cp.handType),
      handProps(cp.handProps) {}

    //@Brief - returns the string representations
    // of all the cards in hand ordered High to low
    // @return std::string
    std::string toString() const;

    //@brief - Add a card to the poker hand; When the 5th card is added.
    // The hand will be evaluated; Trying to add a 6th card will throw an error
    //@param[in] - Card to add to the hand
    //@return void
    void addCard(const Card card);

    //@Brief - Compares the current hand with another hand
    //@Param[in] handToCompare - Hand to compare against
    //@return int - 0 - Hands are equal
    //              1 - The current hand is greater
    //              2 - The handToCompare is greater
    int compare(const PokerHand & handToCompare) const;

    void discardCards(const std::vector<Card> & cardsToRemove);

    Card operator[](std::size_t idx) {
      return hand[idx];
    }

    //Destructor
    ~PokerHand();

    // List of properties that the poker_hand
    // class wants to maintain about the hand
    // These are used for hand comparisons
    struct Props {
      //Value of the three of a kind card for 3oK hands
      unsigned int threeOfKindVal = 0;
      //Values for the 2 pairs of a 2P hand
      std::array<unsigned, 2> twoPairVal;
      //Value of the pair in a 1P hand
      unsigned int onePairVal = 0;
      //Values of the 3ok and 1P parts of a full house
      std::array<unsigned int, 2> fullHouseVal;
      //Value of the 4ok card in a 4ok hand
      unsigned int fourOfAKindVal = 0;
      //List of tiebreaker cards that are used when handtypes match
      std::vector<unsigned int> tieBreakers;
      bool isLowStraight = false;

      Props() {}

      Props(
        const Props & cp) :
        threeOfKindVal(cp.threeOfKindVal),
        twoPairVal(cp.twoPairVal),
        onePairVal(cp.onePairVal),
        fullHouseVal(cp.fullHouseVal),
        fourOfAKindVal(cp.fourOfAKindVal),
        tieBreakers(cp.tieBreakers),
        isLowStraight(cp.isLowStraight) {}
    }; // NOTE from Zac: Wanted to make this a union type so that values are set
    // based on their handtype but I couldn't figure it out.


    //@Brief - Returns the enumeration value for the type
    // of hand the current hand is.
    //@return HandTypeE
    HandTypeE getHandType() const;

    int getNumCardsInHand() const;
  private:
    //@Brief - Returns the properties of the hand
    //@return Props - Properties of the hand
    Props getProperties() const;

    //@Brief - Evaluates the hand and determines the hand properties
    //@return void
    void setHandProperties();

    //@Brief - Determines if the hand is a Straight Flush;
    // Sets hands properties if true
    //@return bool - true if hand is a straight flush; false otherwise
    bool isStraightFlush();

    //@Brief - Determines if the hand is a straight
    // Sets hands properties if true
    //@return bool - true if hand is a straight; false otherwise
    bool isStraight();

    //@Brief - Determines if the hand is a Flush
    // Sets hands properties if true
    //@return bool - true if hand is a flush; false otherwise
    bool isFlush();
    
    //@Brief - Determines if the hand is a full house or a 3ok
    // Sets hands properties if true
    //@return bool - true if hand is a full house; false if hand is 3ok
    bool isFullHouse();

    //@Brief - Determins if the hand is a one pair of a two pair
    //@return bool - true if hand is 1P; false if hand is 2P
    bool isOnePair();

    //@Brief - Determines how many cards have a matching value in the hand with
    // the most occurrences
    //@return int - Number of cards in hand with the same value
    int countDuplicates() const;

    //@Brief - Returns a map that is key - card value, value - number of
    // occurrences in hand.
    //@Return std::map<unsigned int, int> 
    std::map<unsigned int, int> getDuplicateCountMap() const;

    //@Brief - Handles the tiebreaking for each hand type
    //@Param[in] comp - PokerHand the current hand is being compared to
    //@return int - 0 - Hands are equal
    //              1 - The current hand is greater
    //              2 - The handToCompare is greater
    int handleTypeTieBreak(const PokerHand & comp) const;

    //@Brief - Handles tiebreaks on a card by card basis.
    // Cards that are not used in handType values are not used as
    // tiebreakers
    //@Param[in] tieBreakers - tieBreaker cards of the hand being compared to
    // the current hand
    //@return int - 0 - Hands are equal
    //              1 - The current hand is greater
    //              2 - The handToCompare is greater
    int handleTieBreakers(const std::vector<unsigned int> tieBreakers) const;
    std::vector<Card> hand;
    HandTypeE handType;
    PokerHand::Props handProps;
};

//Stream operator for each stream usage in caller
//Prints the contents of the hand in a human readable manner
std::ostream &operator<<(std::ostream &out, const PokerHand hand) {
  out << hand.toString();
  return out; 
}

//Set of operators for handling == < and > for pokerhands
bool operator==(const PokerHand hand1, const PokerHand hand2) {
  return (hand1.compare(hand2) == 0);
}

bool operator<(const PokerHand hand1, const PokerHand hand2) {
  return (hand1.compare(hand2) == 2);
}

bool operator>(const PokerHand hand1, const PokerHand hand2) {
  return (hand1.compare(hand2) == 1);
}

//Stream operator to stringify the enumeration of the hand's type
std::ostream &operator<<(std::ostream &out, const HandTypeE type) {
  switch(type) {
    case HandTypeE::eStraight: {
      out << "Straight" << std::endl;
      break;
    }
    case HandTypeE::eStraightFlush: {
      out << "Straight Flush" << std::endl;
      break;
    }
    case HandTypeE::eOnePair: {
      out << "One Pair" << std::endl;
      break;
    }
    case HandTypeE::eTwoPair: {
      out << "Two Pair" << std::endl;
      break;
    }
    case HandTypeE::eFourOfAKind: {
      out << "Four of a Kind" << std::endl;
      break;
    }
    case HandTypeE::eHighCard: {
      out << "High Card" << std::endl;
      break;
    }
    case HandTypeE::eFullHouse: {
      out << "Full House" << std::endl;
      break;
    }
    case HandTypeE::eThreeOfAKind: {
      out << "Three of a Kind" << std::endl;
      break;
    }
    case HandTypeE::eFlush: {
      out << "Flush" << std::endl;
      break;
    }
    case HandTypeE::eNotEnoughCards: {
      out << "Not Enough Cards" << std::endl;
      break;
    }
    default : {
      out << "PokerHandType doesn't exist" << std::endl;
    }
  }
  return out;
}