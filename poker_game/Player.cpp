#include "Player.h"
Player::Player(const double money,
    const std::string name,
    const int currBet,
    const bool isComputer) :
    currMoney(money),
    currBet(currBet),
    hand(PokerHand()),
    computer(isComputer),
    folded(false),
    name(name),
    winner(false) {}
Player::Player(const Player & cp) :
    currMoney(cp.currMoney),
    currBet(cp.currBet),
    hand(cp.hand),
    computer(cp.computer),
    folded(cp.folded),
    name(cp.name),
    winner(cp.winner){}
Player::~Player(){}
void Player::adjustMoney(double winnings) {
  if (winnings > 0) {
    currMoney += winnings;
  }
  else {
    currMoney -= currBet;
  }
}
void Player::addCard(const Card & card) {
  try {
    hand.addCard(card);
  }
  catch (std::invalid_argument e) {
    throw std::invalid_argument(e.what());
  }
}
std::string Player::toString() const {
  std::stringstream ss;
  ss << name << std::endl;
  ss << "Money Available: " << currMoney << std::endl;
  ss << "Current Bet: " << currBet << std::endl;
  return ss.str();
}
void Player::resetHand() {
  hand = PokerHand();
  folded = false;
  winner = false;
}
bool Player::isWinner() const {
  return winner;
}
void Player::setAsWinner() {
  winner = true;
}
void Player::discardCards(const std::vector<Card> & cards) {
  hand.discardCards(cards);
}
double Player::getCurrBet() const {
  return currBet;
}
double Player::getMoney() const {
  return currMoney;
}
bool Player::isFolded() const {
  return folded;
}
bool Player::isComputer() const {
  return computer;
}
int Player::doComputerAction(const Player::RoundCat & currRound,
  double & currTabBet,
  bool & isOpen) {
  int ret = 0;
  if (not isOpen and getConfidenceInWin() >= 10
    and currRound == RoundCat::BETTING1) {
    currBet = currTabBet;
    isOpen = true;
  }
  else if (not isOpen and getConfidenceInWin() >= 10
    and currRound == RoundCat::BETTING2) {
    currBet = currTabBet;
    isOpen = true;
  }
  else {
    if (currRound == RoundCat::BETTING1 and getConfidenceInWin() <= 10) {
      folded = true;
    } //Fold
    else if (currRound == RoundCat::BETTING2
      and getConfidenceInWin() <= 10) {
      folded = true;
    } //Discard
    else if (currRound == RoundCat::DRAW){
      switch (getConfidenceInWin())
      {
      case 90:
      case 80:
      case 60:
      case 50:
      case 40:
        break;
      case 30:
        ret = 2;
      case 20:
        ret = 3;
      case 12:
        ret = 4;
      default:
        break;
      }
    } //Raise
    else if ((currRound == RoundCat::BETTING1 or 
      currRound == RoundCat::BETTING2) and getConfidenceInWin() > 50) {
      if (currTabBet < currMoney - 10) {
        currBet = currTabBet + 5;
        currTabBet = currBet;
      }
    } //Call
    else if ((currRound == RoundCat::BETTING1 or 
      currRound == RoundCat::BETTING2) and getConfidenceInWin() < 50) {
      if (currTabBet < currMoney - 10) {
        currBet = currTabBet;
        currTabBet = currBet;
      }
    }
  }
  return ret;
}
int Player::getConfidenceInWin() {
  int confidenceInWin = 0;
  switch (hand.getHandType())
  {
  case HandTypeE::eStraight: {
    confidenceInWin = 50;
    break;
  }
  case HandTypeE::eStraightFlush: {
    confidenceInWin = 90;
    break;
  }
  case HandTypeE::eHighCard: {
    if (hand[4].getValue() >= 8) {
      confidenceInWin = 12;
    }
    else {
      confidenceInWin = 10;
    }
    break;
  }
  case HandTypeE::eFlush: {
    confidenceInWin = 60;
    break;
  }
  case HandTypeE::eFourOfAKind: {
    confidenceInWin = 80;
    break;
  }
  case HandTypeE::eThreeOfAKind: {
    confidenceInWin = 40;
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
    confidenceInWin = 70;
    break;
  }
  default:
    break;
  }
  return confidenceInWin;
}
void Player::doAction(
  const PlayerAction & action,
  const double & bet,
  const std::vector<Card> & cardToRemove) {
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
    case PlayerAction::CALL:
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
PokerHand Player::getHand() const {
  return hand;
}
std::string Player::printHand() const {
  std::stringstream ss;
  ss << name << std::endl;
  ss << hand << std::endl;
  return ss.str();
}