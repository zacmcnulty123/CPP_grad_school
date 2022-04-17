#include "Dealer.h"
#include "Player.h"
class PokerTable
{
public:

  const static int MAX_CAPACITY = 7;
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
  ~PokerTable() {}

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

  Player::RoundCat getCurrRound() const {
    return currRound;
  }

  double getAnte() const {
    return currAnte;
  }

  void openTable(const int & openedIdx) {
    opened = true;
    Deck::swap(&playerOrder[0], &playerOrder[openedIdx]);
  }

  std::vector<int> getPlayerOrder() const {
    return playerOrder;
  }

  void addPlayer(const Player & player) {
    if (players.size() >= MAX_CAPACITY+1) {
      throw std::invalid_argument("Table is full");
    }
    else {
      players.push_back(player);
      playerOrder.push_back(playerNum++);
    }
  }

  void dealHands() {
    for (Player & player : players) {
      for (int i = 0; i < 5; ++i) {
        player.addCard(dealer.dealCard());
      }
    }
  }

  void resetRound() {
    opened = false;
    currRound = Player::RoundCat::INIT;
    playerOrder = std::vector<int>();
    int i = 0;
    for (const Player player: players) {
      playerOrder.push_back(i++);
    }
  }

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

  int getNumPlayers() const {
    return players.size();
  }

  std::string getPlayerHand(const int & idx) const {
    if (idx > players.size()) {
      throw std::invalid_argument("Index given outside of range! " 
        + std::to_string(players.size()));
    }
    return players[idx].printHand();
  }

  bool isPlayerComputer(const int & idx) const {
    if (idx > players.size()) {
      throw std::invalid_argument("Index given outside of range! " 
        + std::to_string(players.size()));
    }
    return players[idx].isComputer();
  }

  void doComputerPlayerAction(const int & idx) {
    bool temp = isOpened();
    int numToRemove = players[idx].doComputerAction(currRound, currAnte, temp);
    if ((currRound == Player::RoundCat::BETTING1 
      or currRound == Player::RoundCat::BETTING2)
      and isOpened() == false and temp == true) {
        opened = true;
      }
    else {
      PokerHand temp = players[idx].getHand();
      std::vector<Card> toRemove;
      std::vector<int> cardsToRemove;
      std::srand(time(NULL));
      for (int i = 0; i < numToRemove; ++i) {
        int j = std::rand() % (i+1);
        cardsToRemove.push_back(j);
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

  bool isOpened() const {
    return opened;
  }

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
      //TODO: Probably want to handle Ante's here too
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

  double getPlayerMoney(const int & idx) const {
    if (idx > players.size()) {
      throw std::invalid_argument("Index given outside of range! " 
        + std::to_string(players.size()));
    }
    return players[idx].getMoney();
  }

  double getPlayerBet(const int & idx) const {
    if (idx > players.size()) {
      throw std::invalid_argument("Index given outside of range! " 
        + std::to_string(players.size()));
    }
    return players[idx].getCurrBet();
  }

  Player completeShowdown(bool & tied) {
    Player winner;
    int idx = 0;
    for (Player & player : players) {
      if (not player.isFolded()) {
        winner = player;
      }
      else {
        idx++;
      }
    }
    for (int i = 1; i < players.size(); ++i) {
      if (players[i].isFolded()) {continue;}
      if (winner.getHand() < players[i].getHand()) {
        winner = players[i];
        tied = false;
      }
      else if (winner.getHand() == players[i].getHand()) {
        tied = true;
      }
    }
    return winner;
  }

  bool isPlayerFolded(const int & idx) const {
    if (idx > players.size()) {
      throw std::invalid_argument("Index given outside of range! " 
        + std::to_string(players.size()));
    }
    return players[idx].isFolded();
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

  void validatePlayerIndex(const int & idx) const {
    if (idx > players.size()) {
      throw std::invalid_argument("Index given outside of range! " 
        + std::to_string(players.size()));
    }
  }
};