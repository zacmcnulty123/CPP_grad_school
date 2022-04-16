#pragma once
#include "../poker_hands/poker_hand.cpp"

class Player
{
  enum class playerAction {FOLD, RAISE, CHECK, BET, CALL, DISCARD};
private:
  /* data */
  double currMoney;
  double currBet;
  PokerHand hand;
  bool isComputer;
  int confidenceInWin;
public:
  Player(const double money, const bool isComputer) :
    currMoney(money),
    currBet(0),
    hand(PokerHand()),
    isComputer(isComputer),
    confidenceInWin(0){}
  ~Player() {}

  void addCard(const Card card) {
    try {
      hand.addCard(card);
    }
    catch (std::invalid_argument e) {
      throw std::invalid_argument(e.what());
    }
  }

  void discardCards(const std::vector<Card> cards) {
    hand.discardCards(cards);
  }

  void getConfidenceInWin() {
    switch (hand.getHandType())
    {
    case HandTypeE::eStraight: {
      confidenceInWin = 80;
      break;
    }
    case HandTypeE::eStraightFlush: {
      confidenceInWin = 100;
      break;
    }
    case HandTypeE::eHighCard: {
      confidenceInWin = 10;
      break;
    }
    case HandTypeE::eFlush: {
      confidenceInWin = 50;
      break;
    }
    case HandTypeE::eFourOfAKind: {
      confidenceInWin = 70;
      break;
    }
    case HandTypeE::eThreeOfAKind: {
      confidenceInWin = 50;
      break;
    }
    case HandTypeE::eTwoPair: {
      confidenceInWin = 30;
      break;
    }
    case HandTypeE::eOnePair: {
      confidenceInWin = 20;
      break;
    } 
    case HandTypeE::eFullHouse: {
      confidenceInWin = 60;
      break;
    }
    default:
      break;
    }
  }

  void doAction(const playerAction action, const double bet=0) {

  }
};