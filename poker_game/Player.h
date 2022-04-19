#pragma once
#include "../poker_hands/poker_hand.cpp"
class Player
{
private:
  //Money available to play with
  double currMoney;
  //Current Bet made
  double currBet;
  //Player's hand
  PokerHand hand;
  //If the player is a computer
  bool computer;
  //If the player folded
  bool folded;
  //Name of the player
  std::string name;
  //If the player won the showdown
  bool winner;
public:
  //Category of phase in the poker game. Computer needs this to make decisions
  enum class RoundCat {INIT=0, BETTING1=1, DRAW=2, BETTING2=3, SHOWDOWN=4};
  //Actions available to the player
  enum class PlayerAction {CHECK=1, BET=2, RAISE=3, FOLD=4, CALL=5, DISCARD=6};
  //Constructor
  Player(const double money,
    const std::string name,
    const int currBet=0,
    const bool isComputer=false) :
    currMoney(money),
    currBet(currBet),
    hand(PokerHand()),
    computer(isComputer),
    folded(false),
    name(name),
    winner(false) {}
  //Destructor
  ~Player() {}
  //Copy Constructor
  Player(const Player & cp) :
    currMoney(cp.currMoney),
    currBet(cp.currBet),
    hand(cp.hand),
    computer(cp.computer),
    folded(cp.folded),
    name(cp.name),
    winner(cp.winner){}
  //Default constructor
  Player() {}
  //assignment operator overload
  void operator=(const Player & cp) {
    currMoney = cp.currMoney;
    currBet = cp.currBet;
    hand = cp.hand;
    computer = cp.computer;
    folded = cp.folded;
    name = cp.name;
    winner = cp.winner;
  }
  //@Brief - adjusts the money of the player
  //@Param[in] winnings - amount of money won that player
  void adjustMoney(double winnings=0) {
    if (winnings > 0) {
      currMoney += winnings;
    }
    else {
      currMoney -= currBet;
    }
  }
  //@Brief - adds card to player's hand
  //@Param[in] card - card to add
  void addCard(const Card & card) {
    try {
      hand.addCard(card);
    }
    catch (std::invalid_argument e) {
      throw std::invalid_argument(e.what());
    }
  }
  //@Brief - to string method for the player
  //@return std::string
  std::string toString() const {
    std::stringstream ss;
    ss << name << std::endl;
    ss << "Money Available: " << currMoney << std::endl;
    ss << "Current Bet: " << currBet << std::endl;
    return ss.str();
  }
  //@Brief - resets the player's hand and status
  // for in between rounds
  void resetHand() {
    hand = PokerHand();
    folded = false;
    winner = false;
  }
  //@Brief - returns if the player is the winner of the round
  //@return - bool
  bool isWinner() const {
    return winner;
  }
  //@Brief - sets the player as the winner of the current round
  void setAsWinner() {
    winner = true;
  }
  //@Brief - Discards cards from the player's hand
  //@Param[in] - list of cards to discard
  void discardCards(const std::vector<Card> & cards) {
    hand.discardCards(cards);
  }
  //@Brief - gets the player's current bet
  //@return double
  double getCurrBet() const {
    return currBet;
  }
  //@Brief - gets the money available to the player
  //@return double
  double getMoney() const {
    return currMoney;
  }
  //@Brief - Gets the name of the player
  //@return std::string
  std::string getName() const {
    return name;
  }
  //@Brief - getter for folded
  bool isFolded() const {
    return folded;
  }
  //@Brief - getter for computer
  bool isComputer() const {
    return computer;
  }
  //@Brief - Dumb AI implementation that allows the player to play the game
  //@Param[in] - Current round of the game
  //@Param[in] currTabBet - Current bet of the table
  //@Param[in] isOpen - whether the table is opened or not
  //@return int - Number of cards being discarded: 0 if it isn't the draw round
  int doComputerAction(
    const Player::RoundCat & currRound,
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
  //@Brief - Used for the AI to gauge confidence
  //@return int
  int getConfidenceInWin() {
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
  //@Brief - Handles actions that can be done by the player
  //@Param[in] action - Action the player chose
  //@Param[in] bet - Bet amount chosen by the player
  //@Param[in] cardToRemove - Cards the player chose to discard
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
  //@Brief - Getter for the player's hand
  PokerHand getHand() const {
    return hand;
  }
  //@Brief - ToString method for the player's hand
  std::string printHand() const {
    std::stringstream ss;
    ss << name << std::endl;
    ss << hand << std::endl;
    return ss.str();
  }
};