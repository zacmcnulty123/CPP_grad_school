#include "Dealer.h"
#include "Player.h"
#include <set>
class PokerTable
{
public:
  //Max capacity for the poker table
  const static int MAX_CAPACITY = 7;
  //Constructor
  PokerTable(
    const double currAnte) :
      players(std::vector<Player>()),
      currAnte(currAnte),
      currRound(Player::RoundCat::INIT),
      openerPos(0),
      opened(false),
      playerNum(0),
      playerOrder(std::vector<int>()) {
        dealer = Dealer();
        minAnte = currAnte;
      }
  PokerTable() : 
      players(std::vector<Player>()),
      currAnte(0),
      currRound(Player::RoundCat::INIT),
      openerPos(0),
      opened(false),
      playerNum(0),
      playerOrder(std::vector<int>()) {
        dealer = Dealer();
      }
  //Destructor
  ~PokerTable() {}
  //@Brief - Sets the ante of the table
  //@Param[in] ante - value to set
  void setTableAnte(const double & ante) {
    currAnte = ante;
  }
  //Advances the phase of the game
  void advanceRound() {
    switch (currRound)
    {
      case Player::RoundCat::INIT: {
        currRound = Player::RoundCat::BETTING1;
        break;
      }
      case Player::RoundCat::BETTING1: {
        currRound = Player::RoundCat::DRAW;
        break;
      }
      case Player::RoundCat::DRAW: {
        currRound = Player::RoundCat::BETTING2;
        opened = false;
        break;
      }
      case Player::RoundCat::BETTING2: {
        currRound = Player::RoundCat::SHOWDOWN;
        break;
      }
      case Player::RoundCat::SHOWDOWN: {
        currRound = Player::RoundCat::INIT;
      }
      default:
        break;
    }
  }
  //@Brief - getter for currRound
  Player::RoundCat getCurrRound() const {
    return currRound;
  }
  //@Brief - getter fot the ante
  double getAnte() const {
    return currAnte;
  }
  //@Brief - puts the table into the open state
  //@Param[in] openedIdx - Index of the player who opened the betting
  void openTable(const int & openedIdx) {
    opened = true;
    if (currRound == Player::RoundCat::BETTING1) {
      Deck::swap(&playerOrder[0], &playerOrder[openedIdx]);
    }
  }
  //@Brief - getter for playerOrder
  std::vector<int> getPlayerOrder() const {
    return playerOrder;
  }
  //@Brief - Adds player to the table
  //@Param[in] money - money the player has available
  //@Param[in] name - name of the player
  //@param[in] isComputer - whether the player is a computer or not
  void addPlayer(const double money, const std::string name,
    bool isComputer=false) {
    if (players.size() >= MAX_CAPACITY) {
      throw std::invalid_argument("Table is full");
    }
    else {
      players.push_back(Player(money, name, getAnte(), isComputer));
      playerOrder.push_back(playerNum++);
    }
  }
  //@Brief - deals hands to all of the players
  void dealHands() {
    for (Player & player : players) {
      for (int i = 0; i < 5; ++i) {
        player.addCard(dealer.dealCard());
      }
    }
  }
  //@Brief - resets the table to the initial round
  void resetRound() {
    opened = false;
    currRound = Player::RoundCat::INIT;
    playerOrder = std::vector<int>();
    dealer.makeNewDeck();
    int i = 0;
    for (const Player player: players) {
      playerOrder.push_back(i++);
    }
    for (Player & player : players) {
      player.resetHand();
    }
  }
  //@Brief - Reveals the hand of each player
  //@return std::string
  std::string showPlayerHands() const {
    std::stringstream ss;
    int i = 1;
    for (const Player & player : players) {
      //TODO: Only show human players once this all works
      ss << "Player" << i++ << " Hand: " << std::endl; 
      ss << player.getHand() << std::endl;
    }
    return ss.str();
  }
  //@Brief - returns the number of players sitting at the table
  int getNumPlayers() const {
    return players.size();
  }
  //@Brief - prints the given player's hand
  //@Param[in] idx - seat of the player
  //@return std::string
  std::string getPlayerHand(const int & idx) const {
    if (idx > players.size()) {
      throw std::invalid_argument("Index given outside of range! " 
        + std::to_string(players.size()));
    }
    return players[idx].printHand();
  }
  //@Brief - Tells if the given player is a computer
  //@Param[in] idx - seat of the player
  //@return bool
  bool isPlayerComputer(const int & idx) const {
    if (idx > players.size()) {
      throw std::invalid_argument("Index given outside of range! " 
        + std::to_string(players.size()));
    }
    return players[idx].isComputer();
  }
  //@Brief - does dumb AI action
  //@Paramp[in] idx - seat of the player
  void doComputerPlayerAction(const int & idx) {
    bool temp = isOpened();
    int numToRemove = players[idx].doComputerAction(currRound, currAnte, temp);
    if ((currRound == Player::RoundCat::BETTING1 
      or currRound == Player::RoundCat::BETTING2)
      and isOpened() == false and temp == true) {
        opened = true;
      }
    else if (currRound == Player::RoundCat::DRAW) {
      if (numToRemove > 0) {
        PokerHand temp = players[idx].getHand();
        std::vector<Card> toRemove;
        std::set<int> cardsToRemove;
        std::srand(time(NULL));
        int k = 0;
        while (cardsToRemove.size() < numToRemove) {
          if (k > cardsToRemove.size()) {k=0;}
          int j = std::rand() % (k+1);
          cardsToRemove.insert(j);
          k++;
        }
        for (const int & i : cardsToRemove) {
          toRemove.push_back(temp[i]);
        }
        players[idx].doAction(
          Player::PlayerAction::DISCARD,
          0,
          toRemove);
        dealer.addToDiscardPile(toRemove);
        while (players[idx].getHand().getNumCardsInHand() < 5) {
          players[idx].addCard(dealer.dealCard());
        }
      }
    }
  }
  //@Brief - getter for opened
  bool isOpened() const {
    return opened;
  }
  //@Brief - commits the action of a human player
  //@Param[in] idx - seat of the player
  //@Param[in] actionIdx - Action chosen by the player
  //@Param[in] bet - bet chosen by the player
  //@Param[in] cardsToRemove - cards player chose to discard
  void commitPlayerAction (
    const int & idx,
    const int & actionIdx,
    const double & bet=0,
    const std::vector<int> & cardsToRemove=std::vector<int>()) {
    if (idx > players.size()) {
      throw std::invalid_argument("Index given outside of range! " 
        + std::to_string(players.size()));
    }
    try {
      if (static_cast<Player::PlayerAction>(actionIdx)
          == Player::PlayerAction::RAISE) {
        if (bet < currAnte) {
          throw std::invalid_argument("Cannot place a "
            "bet lower than the table's highest bet:" +std::to_string(currAnte));
        }
      }
      if (not opened) {
        currAnte = bet;
      }
      else if (currAnte < bet) {
        currAnte = bet;
      }
      if (static_cast<Player::PlayerAction>(actionIdx) ==
          Player::PlayerAction::CALL) {
          players[idx].doAction(
            static_cast<Player::PlayerAction>(actionIdx),
            currAnte);
      }
      if (static_cast<Player::PlayerAction>(actionIdx) ==
          Player::PlayerAction::DISCARD) {
        PokerHand temp = players[idx].getHand();
        std::vector<Card> toRemove;
        for (const int & i : cardsToRemove) {
          toRemove.push_back(temp[i]);
        }
        players[idx].doAction(
          static_cast<Player::PlayerAction>(actionIdx),
          bet,
          toRemove);
        dealer.addToDiscardPile(toRemove);
        while (players[idx].getHand().getNumCardsInHand() < 5) {
          players[idx].addCard(dealer.dealCard());
        }
      }
      else {
        players[idx].doAction(
          static_cast<Player::PlayerAction>(actionIdx),
          bet);
      }
    }
    catch (std::invalid_argument e) {
      throw std::invalid_argument(e.what());
    }
  }
  //@Brief - getPlayer's money available
  //@Param[in] idx - seat of the player
  //@return double
  double getPlayerMoney(const int & idx) const {
    if (idx > players.size()) {
      throw std::invalid_argument("Index given outside of range! " 
        + std::to_string(players.size()));
    }
    return players[idx].getMoney();
  }
  //@Brief - Gets the given player's bet
  //@Param[in] idx - seat of the player
  //@return - double
  double getPlayerBet(const int & idx) const {
    if (idx > players.size()) {
      throw std::invalid_argument("Index given outside of range! " 
        + std::to_string(players.size()));
    }
    return players[idx].getCurrBet();
  }
  //@Brief - Does the showdown phase of the game
  //@Param[in] whether there is a tie
  //@return Player
  Player completeShowdown(bool & tied) {
    tied = true;
    Player winner;
    std::vector<int> idx;
    int temp = 0;
    for (Player & player : players) {
      if (not player.isFolded()) {
        winner = player;
        idx.push_back(temp++);
      }
      else {
        temp++;
      }
    }
    if (idx.size() <= 1) {
      //Do nothing
    }
    else {
      for (const int & i : idx) {
        if (winner.getHand() < players[i].getHand()) {
          winner = players[i];
          temp = i;
          tied = false;
        }
        else if (winner.getHand() == players[i].getHand()) {
          tied = true;
        }
      }
    }
    players[temp].setAsWinner();
    return winner;
  }
  //@Brief - determines if all player's folded
  //@return bool
  bool allPlayersFolded() const {
    bool ret = true;
    for (const Player & player : players) {
      if (not player.isFolded()) {
        ret = false;
        break;
      }
    }
    return ret;
  }
  //@Brief - returns if a given player folded
  //@Param[in] idx - seat of the player
  //@return bool
  bool isPlayerFolded(const int & idx) const {
    if (idx > players.size()) {
      throw std::invalid_argument("Index given outside of range! " 
        + std::to_string(players.size()));
    }
    return players[idx].isFolded();
  }
  //@Brief - adjusts the money for each player and removes them from
  // the table if they cannot pay the ante
  void adjustPlayerMoney() {
    double winnings = 0;
    for (Player & player : players) {
      if (player.isFolded() or not player.isWinner()) {
        winnings += player.getCurrBet();
        player.adjustMoney();
      }
    }
    for (Player & player : players) {
      if (player.isWinner()) {
        player.adjustMoney(winnings);
        break;
      }
    }
    std::vector<Player>::iterator it = players.begin();
    for (Player & player : players) {
      if (player.getMoney() < minAnte) {
        players.erase(it);
      }
      else {
        it++;
      }
    }
  }
  //@Brief - prints the information fro a given player
  std::string getPlayerinfo(const int & idx) const {
    if (idx > players.size()) {
      throw std::invalid_argument("Index given outside of range! " 
        + std::to_string(players.size()));
    }
    return players[idx].toString();
  }
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