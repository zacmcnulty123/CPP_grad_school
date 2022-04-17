#include "Dealer.h"
#include "Player.h"
class PokerTable
{
public:
  enum class SeatCat {DEALER, PLAYER};

  enum class RoundCat {INIT=0, BETTING1, DRAW, BETTING2, SHOWDOWN};
  const static int MAX_CAPACITY = 7;
  PokerTable(
    const double currAnte) :
      players(std::vector<Player>()),
      currAnte(currAnte),
      currRound(RoundCat::INIT),
      openerPos(0),
      opened(false),
      blindPos(0),
      currBet(0) {
        dealer = Dealer();
      }
  ~PokerTable() {}

  void advanceRound() {
    switch (currRound)
    {
      case RoundCat::INIT: {
        currRound = RoundCat::BETTING1;
        break;
      }
      case RoundCat::BETTING1: {
        currRound = RoundCat::DRAW;
        break;
      }
      case RoundCat::DRAW: {
        currRound = RoundCat::BETTING2;
        break;
      }
      case RoundCat::BETTING2: {
        currRound = RoundCat::SHOWDOWN;
        break;
      }
      case RoundCat::SHOWDOWN: {
        currRound = RoundCat::INIT;
      }
      default:
        break;
    }
  }

  RoundCat getCurrRound() const {
    return currRound;
  }

  double getAnte() const {
    return currAnte;
  }

  void openTable(const int & openedIdx) {
    opened = true;
    openerPos = openedIdx;
  }

  void addPlayer(const Player & player) {
    if (players.size() >= MAX_CAPACITY+1) {
      throw std::invalid_argument("Table is full");
    }
    else {
      players.push_back(player);
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
    currRound = RoundCat::INIT;
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
        if (bet < currBet) {
          throw std::invalid_argument("Cannot place a "
            "bet lower than the table's highest bet:" +std::to_string(currBet));
        }
      }
      if (not opened) {
        currBet = bet;
      }
      else if (currBet < bet) {
        currBet = bet;
      }
      if (static_cast<Player::PlayerAction>(actionIdx) ==
          Player::PlayerAction::CALL) {
          players[idx].doAction(
            static_cast<Player::PlayerAction>(actionIdx),
            currBet);
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

  bool isPlayerFolded(const int & idx) const {
    if (idx > players.size()) {
      throw std::invalid_argument("Index given outside of range! " 
        + std::to_string(players.size()));
    }
    return players[idx].isFolded();
  }
private:
  std::vector<Player> players;
  double currAnte;
  RoundCat currRound;
  int openerPos;
  bool opened;
  int blindPos;
  double currBet;
  Dealer dealer;

  void validatePlayerIndex(const int & idx) const {
    if (idx > players.size()) {
      throw std::invalid_argument("Index given outside of range! " 
        + std::to_string(players.size()));
    }
  }
};