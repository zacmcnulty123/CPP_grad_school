#include "Dealer.cpp"
#include "Player.cpp"
#include <set>
class PokerTable
{
public:
  //Max capacity for the poker table
  const static int MAX_CAPACITY = 7;
  //Constructor
  PokerTable(const double currAnte);
  PokerTable();
  //Destructor
  ~PokerTable();
  //@Brief - Sets the ante of the table
  //@Param[in] ante - value to set
  void setTableAnte(const double & ante);
  //Advances the phase of the game
  void advanceRound();
  //@Brief - getter for currRound
  Player::RoundCat getCurrRound() const;
  //@Brief - getter fot the ante
  double getAnte() const;
  //@Brief - puts the table into the open state
  //@Param[in] openedIdx - Index of the player who opened the betting
  void openTable(const int & openedIdx);
  //@Brief - getter for playerOrder
  std::vector<int> getPlayerOrder() const;
  //@Brief - Adds player to the table
  //@Param[in] money - money the player has available
  //@Param[in] name - name of the player
  //@param[in] isComputer - whether the player is a computer or not
  void addPlayer(const double money, const std::string name,
    bool isComputer=false);
  //@Brief - deals hands to all of the players
  void dealHands();
  //@Brief - resets the table to the initial round
  void resetRound();
  //@Brief - Reveals the hand of each player
  //@return std::string
  std::string showPlayerHands() const; 
  //@Brief - returns the number of players sitting at the table
  int getNumPlayers() const {
    return players.size();
  }
  //@Brief - prints the given player's hand
  //@Param[in] idx - seat of the player
  //@return std::string
  std::string getPlayerHand(const int & idx) const;
  //@Brief - Tells if the given player is a computer
  //@Param[in] idx - seat of the player
  //@return bool
  bool isPlayerComputer(const int & idx) const;
  //@Brief - does dumb AI action
  //@Paramp[in] idx - seat of the player
  void doComputerPlayerAction(const int & idx);
  //@Brief - getter for opened
  bool isOpened() const;
  //@Brief - commits the action of a human player
  //@Param[in] idx - seat of the player
  //@Param[in] actionIdx - Action chosen by the player
  //@Param[in] bet - bet chosen by the player
  //@Param[in] cardsToRemove - cards player chose to discard
  void commitPlayerAction(
    const int & idx,
    const int & actionIdx,
    const double & bet=0,
    const std::vector<int> & cardsToRemove=std::vector<int>());
  //@Brief - getPlayer's money available
  //@Param[in] idx - seat of the player
  //@return double
  double getPlayerMoney(const int & idx) const;
  //@Brief - Gets the given player's bet
  //@Param[in] idx - seat of the player
  //@return - double
  double getPlayerBet(const int & idx) const;
  //@Brief - Does the showdown phase of the game
  //@Param[in] whether there is a tie
  //@return Player
  Player completeShowdown(bool & tied);
  //@Brief - determines if all player's folded
  //@return bool
  bool allPlayersFolded() const;
  //@Brief - returns if a given player folded
  //@Param[in] idx - seat of the player
  //@return bool
  bool isPlayerFolded(const int & idx) const;
  //@Brief - adjusts the money for each player and removes them from
  // the table if they cannot pay the ante
  void adjustPlayerMoney();
  //@Brief - prints the information fro a given player
  std::string getPlayerInfo(const int & idx) const;
private:
  std::vector<Player> players;
  Player::RoundCat currRound;
  int openerPos;
  bool opened;
  int playerNum;
  double currAnte;
  double minAnte;
  std::vector<int> playerOrder;
  Dealer dealer;
};