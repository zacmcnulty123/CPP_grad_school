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
    const bool isComputer=false);
  //Destructor
  ~Player();
  //Copy Constructor
  Player(const Player & cp);
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
  void adjustMoney(double winnings=0);
  //@Brief - adds card to player's hand
  //@Param[in] card - card to add
  void addCard(const Card & card);
  //@Brief - to string method for the player
  //@return std::string
  std::string toString() const;
  //@Brief - resets the player's hand and status
  // for in between rounds
  void resetHand();
  //@Brief - returns if the player is the winner of the round
  //@return - bool
  bool isWinner() const;
  //@Brief - sets the player as the winner of the current round
  void setAsWinner();
  //@Brief - Discards cards from the player's hand
  //@Param[in] - list of cards to discard
  void discardCards(const std::vector<Card> & cards);
  //@Brief - gets the player's current bet
  //@return double
  double getCurrBet() const;
  //@Brief - gets the money available to the player
  //@return double
  double getMoney() const;
  //@Brief - Gets the name of the player
  //@return std::string
  std::string getName() const;
  //@Brief - getter for folded
  bool isFolded() const;
  //@Brief - getter for computer
  bool isComputer() const;
  //@Brief - Dumb AI implementation that allows the player to play the game
  //@Param[in] - Current round of the game
  //@Param[in] currTabBet - Current bet of the table
  //@Param[in] isOpen - whether the table is opened or not
  //@return int - Number of cards being discarded: 0 if it isn't the draw round
  int doComputerAction(
    const Player::RoundCat & currRound,
    double & currTabBet,
    bool & isOpen);
  //@Brief - Used for the AI to gauge confidence
  //@return int
  int getConfidenceInWin();
  //@Brief - Handles actions that can be done by the player
  //@Param[in] action - Action the player chose
  //@Param[in] bet - Bet amount chosen by the player
  //@Param[in] cardToRemove - Cards the player chose to discard
  void doAction(
    const PlayerAction & action,
    const double & bet=0,
    const std::vector<Card> & cardToRemove=std::vector<Card>());
  //@Brief - Getter for the player's hand
  PokerHand getHand() const;
  //@Brief - ToString method for the player's hand
  std::string printHand() const;
};