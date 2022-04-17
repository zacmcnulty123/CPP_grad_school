#include "Dealer.h"
#include "Player.h"
#include <set>
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
    dealer.makeNewDeck();
    int i = 0;
    for (const Player player: players) {
      playerOrder.push_back(i++);
    }
    for (Player & player : players) {
      player.resetHand();
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
    tied = true;
    Player winner;
    std::vector<int> idx;
    int temp = 0;
    for (Player & player : players) {
      if (not player.isFolded()) {
        winner = Player(player);
        idx.push_back(temp++);
      }
      else {
        temp++;
      }
    }
    if (idx.size() <= 1) {

    }
    else {
      for (const int & i : idx) {
        if (winner.getHand() < players[i].getHand()) {
          winner = Player(players[i]);
          tied = false;
        }
        else if (winner.getHand() == players[i].getHand()) {
          tied = true;
        }
      }
    }
    return winner;
  }

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