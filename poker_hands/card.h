#pragma once
#include <map>
#include <array>
#include <string>
#include<iostream>
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

typedef struct {
  unsigned int value;
  std::string displayValue;
  std::string suit; 
} Card;

enum HandTypeE {eNotEnoughCards,
                eStraight,
                eStraightFlush,
                eOnePair,
                eTwoPair,
                eThreeOfAKind,
                eFourOfAKind,
                eFullHouse,
                eHighCard};

bool operator<(const Card card1, const Card card2) {
  return (card1.value < card2.value);
}

bool operator==(const Card card1, const Card card2) {
  if ((card1.value == card2.value)) {
    return true;
  }
  else {
    return false;
  }
}

std::ostream &operator<<(std::ostream &out, const Card card) {
  out << card.displayValue << card.suit << std::endl;
  return out;
}

std::ostream &operator<<(std::ostream &out, const HandTypeE type) {
  switch(type) {
    case eStraight: {
      out << "Straight" << std::endl;
      break;
    }
    case eStraightFlush: {
      out << "StraightFlush" << std::endl;
      break;
    }
    case eOnePair: {
      out << "OnePair" << std::endl;
      break;
    }
    case eTwoPair: {
      out << "TwoPair" << std::endl;
      break;
    }
    case eFourOfAKind: {
      out << "Four of a Kind" << std::endl;
      break;
    }
    case eHighCard: {
      out << "HighCard" << std::endl;
      break;
    }
    case eFullHouse: {
      out << "FullHouse" << std::endl;
      break;
    }
    case eThreeOfAKind: {
      out << "Three of a Kind" << std::endl;
      break;
    }
    default : {
      out << "Type doesn't exist" << std::endl;
    }
  }
  return out;
}