#pragma once
#include<map>
#include<array>
#include<string>
#include<iostream>
#include<sstream>
//Only valid types
const std::map<std::string, unsigned int> CARD_TO_VALUE_MAP = {
  std::make_pair("2", 2),
  std::make_pair("3", 3),
  std::make_pair("4", 4),
  std::make_pair("5", 5),
  std::make_pair("6", 6),
  std::make_pair("7", 7),
  std::make_pair("8", 8),
  std::make_pair("9", 9),
  std::make_pair("10", 10),
  std::make_pair("J", 11),
  std::make_pair("Q", 12),
  std::make_pair("K", 13),
  //Aces can be counted low in poker for straights
  //But recognizing that is outside this classes responsibilities
  //So default to the highest value possible
  std::make_pair("A", 14)};

//Suits of a card
const std::array<std::string, 4> SUITS = {
  "S", "H", "D", "C"};

class Card {
  public:
    //@Brief - Constructor for the Card Class
    //@Param[in] val - String for the card rank
    //@Param[in] st - String for the card's suit
    Card(std::string val, std::string st) {
      //Ensure that the card is a valid card
      if ((CARD_TO_VALUE_MAP.count(val)) and 
      std::find(std::begin(SUITS), std::end(SUITS), st) != std::end(SUITS)) {
          value = CARD_TO_VALUE_MAP.at(val);
          displayValue = val;
          suit = st;
      }
      else {
        //Throw an error if value arguments are entered.
        throw std::invalid_argument("Arguments given at invalid");
      }
    }
    //Destructor
    ~Card() {}

    //@Brief - Gets the Value of the Card
    //@Return unsigned int - Raw value of the card
    unsigned int getValue() const {
      return value;
    }

    //@Brief - Returns a string representation for the card
    //@return std::string - String representation for the card
    std::string toString() const {
      std::stringstream ss;
      ss << displayValue << suit;
      return ss.str();
    }

    //@Brief - Returns the suit for the card
    //@Return std::string - returns the suit for the card
    std::string getSuit() const {
      return suit;
    }
  private:
    //Raw value of the card
    unsigned int value;
    //Representation of the value of the card
    std::string displayValue;
    //Suit of the card
    std::string suit;
};

// Want to overload this operator so that STL
// functions like std::sort can be used via other classes
bool operator<(const Card card1, const Card card2) {
  return (card1.getValue() < card2.getValue());
}

// Want to overload this operator for the same reasons stated
// in the < operator
bool operator==(const Card card1, const Card card2) {
  if ((card1.getValue() == card2.getValue())) {
    return true;
  }
  else {
    return false;
  }
}

//Stream operator overload for quality of life of classes that 
// want to use the card class.
std::ostream &operator<<(std::ostream &out, const Card card) {
  out << card.toString();
  return out;
}