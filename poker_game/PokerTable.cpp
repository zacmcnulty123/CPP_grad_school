#include "PokerTable.h"
PokerTable::PokerTable(const double currAnte) :
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
PokerTable::PokerTable() : 
  players(std::vector<Player>()),
  currAnte(0),
  currRound(Player::RoundCat::INIT),
  openerPos(0),
  opened(false),
  playerNum(0),
  playerOrder(std::vector<int>()) {
    dealer = Dealer();
}
PokerTable::~PokerTable(){}
void PokerTable::setTableAnte(const double & ante) {
  currAnte = ante;
}
void PokerTable::advanceRound() {
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
Player::RoundCat PokerTable::getCurrRound() const {
  return currRound;
}
double PokerTable::getAnte() const {
  return currAnte;
}
void PokerTable::openTable(const int & openedIdx) {
  opened = true;
  if (currRound == Player::RoundCat::BETTING1) {
    Deck::swap(&playerOrder[0], &playerOrder[openedIdx]);
  }
}
std::vector<int> PokerTable::getPlayerOrder() const {
  return playerOrder;
}
void PokerTable::addPlayer(const double money, const std::string name,
  bool isComputer) {
  if (players.size() >= MAX_CAPACITY) {
    throw std::invalid_argument("Table is full");
  }
  else {
    players.push_back(Player(money, name, getAnte(), isComputer));
    playerOrder.push_back(playerNum++);
  }
}
void PokerTable::dealHands() {
  for (Player & player : players) {
    for (int i = 0; i < 5; ++i) {
      player.addCard(dealer.dealCard());
    }
  }
}
void PokerTable::resetRound() {
  opened = false;
  currRound = Player::RoundCat::INIT;
  playerOrder = std::vector<int>();
  dealer.makeNewDeck();
  multiWin = 1;
  currAnte = minAnte;
  int i = 0;
  for (const Player player: players) {
    playerOrder.push_back(i++);
  }
  for (Player & player : players) {
    player.resetHand();
  }
}
std::string PokerTable::showPlayerHands() const {
  std::stringstream ss;
  int i = 1;
  for (const Player & player : players) {
    //TODO: Only show human players once this all works
    ss << "Player" << i++ << " Hand: " << std::endl; 
    ss << player.getHand() << std::endl;
  }
  return ss.str();
}
std::string PokerTable::getPlayerHand(const int & idx) const {
  if (idx > players.size()) {
    throw std::invalid_argument("Index given outside of range! " 
      + std::to_string(players.size()));
  }
  return players[idx].printHand();
}
bool PokerTable::isPlayerComputer(const int & idx) const {
  if (idx > players.size()) {
    throw std::invalid_argument("Index given outside of range! " 
      + std::to_string(players.size()));
  }
  return players[idx].isComputer();
}
void PokerTable::doComputerPlayerAction(const int & idx) {
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
bool PokerTable::isOpened() const {
  return opened;
}
void PokerTable::commitPlayerAction(
  const int & idx,
  const int & actionIdx,
  const double & bet,
  const std::vector<int> & cardsToRemove) {
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
double PokerTable::getPlayerMoney(const int & idx) const {
  if (idx > players.size()) {
    throw std::invalid_argument("Index given outside of range! " 
      + std::to_string(players.size()));
  }
  return players[idx].getMoney();
}
double PokerTable::getPlayerBet(const int & idx) const {
  if (idx > players.size()) {
    throw std::invalid_argument("Index given outside of range! " 
      + std::to_string(players.size()));
  }
  return players[idx].getCurrBet();
}
std::vector<Player> PokerTable::completeShowdown(bool & tied) {
  tied = true;
  Player winner(players[0]);
  std::vector<int> idx;
  std::vector<Player> ties;
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
        ties = std::vector<Player>();
      }
      else if (winner.getHand() == players[i].getHand()) {
        tied = true;
        ties.push_back(winner);
        ties.push_back(players[i]);
      }
    }
  }
  if (tied) {
    for (Player & player : ties) {
      player.setAsWinner();
      multiWin += 1;
    }
  }
  else {
    players[temp].setAsWinner();
    ties.push_back(players[temp]);
  }
  return ties;
}
bool PokerTable::allPlayersFolded() const {
  bool ret = true;
  for (const Player & player : players) {
    if (not player.isFolded()) {
      ret = false;
      break;
    }
  }
  return ret;
}
bool PokerTable::isPlayerFolded(const int & idx) const {
  if (idx > players.size()) {
    throw std::invalid_argument("Index given outside of range! " 
      + std::to_string(players.size()));
  }
  return players[idx].isFolded();
}
void PokerTable::adjustPlayerMoney() {
  double winnings = 0;
  for (Player & player : players) {
    if (player.isFolded() or not player.isWinner()) {
      winnings += player.getCurrBet();
      player.adjustMoney();
    }
  }
  for (Player & player : players) {
    if (player.isWinner()) {
      //Split the pot according to the number of
      //Players tied
      player.adjustMoney(winnings/multiWin);
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
std::string PokerTable::getPlayerInfo(const int & idx) const {
  if (idx > players.size()) {
    throw std::invalid_argument("Index given outside of range! " 
      + std::to_string(players.size()));
  }
  return players[idx].toString();
}