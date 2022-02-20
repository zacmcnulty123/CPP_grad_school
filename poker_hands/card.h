#pragma once
#include <map>
#include <array>
#include <string>
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
  std::make_pair("A", 14)};

//Suits of a card
const std::array<std::string, 4> SUITS = {
  "S", "H", "D", "C"};

class Card {
  public:
    Card(std::string val, std::string st) {
      if ((CARD_TO_VALUE_MAP.count(val)) and 
      std::find(std::begin(SUITS), std::end(SUITS), st) != std::end(SUITS)) {
          value = CARD_TO_VALUE_MAP.at(val);
          displateValue = val;
          suit = st;
      }
      else {
        throw std::invalid_argument("Arguments given at invalid");
      }
    }
    ~Card() {}
    unsigned int getValue() const {
      return value;
    }
    std::string to_string() const {
      std::stringstream ss;
      ss << displateValue << suit;
      return ss.str();
    }
    std::string getSuit() const {
      return suit;
    }
  private:
    unsigned int value;
    std::string displateValue;
    std::string suit;
};

bool operator<(const Card card1, const Card card2) {
  return (card1.getValue() < card2.getValue());
}

bool operator==(const Card card1, const Card card2) {
  if ((card1.getValue() == card2.getValue())) {
    return true;
  }
  else {
    return false;
  }
}

std::ostream &operator<<(std::ostream &out, const Card card) {
  out << card.to_string();
  return out;
}