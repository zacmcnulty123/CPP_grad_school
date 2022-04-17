#pragma once
#include "../poker_hands/poker_hand.cpp"

class Player
{
private:
  /* data */
  double currMoney;
  double currBet;
  PokerHand hand;
  bool computer;
  int confidenceInWin;
  bool folded;
  std::string name;
public:
  enum class PlayerAction {CHECK=1, BET=2, RAISE=3, FOLD=4, CALL=5, DISCARD=6};
  Player(const double money,
    const std::string name,
    const bool isComputer=false) :
    currMoney(money),
    currBet(0),
    hand(PokerHand()),
    computer(isComputer),
    confidenceInWin(0),
    folded(false),
    name(name) {}
  ~Player() {}

  Player() {}

  void addCard(const Card & card) {
    try {
      hand.addCard(card);
    }
    catch (std::invalid_argument e) {
      throw std::invalid_argument(e.what());
    }
  }

  void discardCards(const std::vector<Card> & cards) {
    hand.discardCards(cards);
  }

  double getCurrBet() const {
    return currBet;
  }

  double getMoney() const {
    return currMoney;
  }

  bool isFolded() const {
    return folded;
  }

  bool isComputer() const {
    return computer;
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

  void doAction(
    const PlayerAction & action,
    const double & bet=0,
    const std::vector<Card> & cardToRemove=std::vector<Card>()) {
    if (bet > currMoney) {
      throw std::invalid_argument("Bet cannot be higher than the "
        "total money you have: " + std::to_string(currMoney));
    }
    else if (bet < currBet and bet != 0) {
      throw std::invalid_argument("Bet cannot be lower"
        "than current bet of: " + std::to_string(currBet));
    }
    switch (action)
    {
      case PlayerAction::FOLD: {
        folded = true;
        break;
      }
      case PlayerAction::RAISE:
      case PlayerAction::BET: {
        currBet = bet;
        break;
      }
      case PlayerAction::CHECK: {
        //do nothing
        break;
      }

      case PlayerAction::DISCARD: {
        discardCards(cardToRemove);
        break;
      }
      default: {
        throw std::invalid_argument("PlayerAction Unknown!");
      }
    }
  }

  PokerHand getHand() const {
    return hand;
  }

  std::string printHand() const {
    std::stringstream ss;
    ss << name << std::endl;
    ss << hand << std::endl;
    return ss.str();
  }
};

std::ostream &operator <<(std::ostream & out, const Player::PlayerAction action) {
  switch (action)
  {
    case Player::PlayerAction::CHECK: {
      out << "Check" << std::endl;
      break;
    }
    case Player::PlayerAction::BET: {
      out << "BET" << std::endl;
      break;
    }
    case Player::PlayerAction::RAISE: {
      out << "RAISE" << std::endl;
      break;
    }
    case Player::PlayerAction::FOLD: {
      out << "FOLD" << std::endl;
      break;
    }
    case Player::PlayerAction::CALL: {
      out << "CALL" << std::endl;
      break;
    }
    case Player::PlayerAction::DISCARD: {
      out << "DISCARD" << std::endl;
    }
    default:
      break;
  }
  return out;
}