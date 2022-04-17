#include "PokerTable.h"
#include "../helpers/input_helper.h"
using namespace std;

void handleOpenTable(PokerTable & table, const int & i) {
  bool validInput = false;
  while (not validInput) {
    cout << "You currently have: " << table.getPlayerMoney(i)
    << "\n The Table's current Bet is: " << table.getAnte()
    << "\nYour current bet is: " << table.getPlayerBet(i) << endl;
    cout << "Would you like to \n" <<
    "Enter 3 to Raise \n" <<
    "Enter 4 to Fold\n" <<
    "Enter 5 to Call\n";
    std::string input;
    cin >> input;
    if (input_helper::is_number(input)) {
      try {
        if (std::stod(input) == 3) {
          cout << "Enter Bet Amount: " << endl;
          std::string betAmt;
          cin >> betAmt;
          if (input_helper::is_pos_number(input)) {
            table.commitPlayerAction(i, 3, std::stod(betAmt));
            validInput = true;
          }
        }
        else if (std::stod(input) == 4) {
          table.commitPlayerAction(i, 4);
          validInput = true;
        }
        else if (std::stod(input) == 5) {
          table.commitPlayerAction(i, 5);
          validInput = true;
        }
      } catch (std::invalid_argument e) {
        cerr << "Caught Exception: " << e.what() << endl;
      } catch (std::exception e) {
        cerr << "Caught Exception! " << e.what() << endl;
      }
    }
    if (not validInput) {
      cout << "Invalid input, please try again" << endl;
    }
  }
}

void handleNotOpenTable(PokerTable & table, const int i) {
  bool validInput = false;
  while (not validInput) {
    cout << "You currently have: " << table.getPlayerMoney(i)
    << "\n The Table's current Bet is: " << table.getAnte()
    << "\nYour current bet is: " << table.getPlayerBet(i) << endl;
    cout << "Would you like to \n" <<
    "Enter 1 to Check \n" <<
    "Enter 2 to Bet \n";

    std::string input;
    cin >> input;
    if (input_helper::is_number(input)) {
      try {
        if (std::stod(input) == 1) {
          table.commitPlayerAction(i, 1);
          validInput = true;
        }
        else if (std::stod(input) == 2) {
          cout << "Enter Bet Amount: " << endl;
          std::string betAmt;
          cin >> betAmt;
          if (input_helper::is_pos_number(input)) {
            table.commitPlayerAction(i, 2, std::stod(betAmt));
            validInput = true;
            table.openTable(i);
          }
        }
      } catch (std::exception e) {
        cerr << "Caught Exception!" << e.what() << endl;
      }
    }
    if (not validInput) {
      cout << "Invalid input, please try again" << endl;
    }
  }
}

void handleDrawPhase(PokerTable & table, const int & idx) {
  cout << table.getPlayerHand(idx);
  const char* separator = "";
  for (int i = 1; i <= 5; ++i) {
    cout << separator << " " <<  i;
    separator = ",";
  }
  cout << "\n";
  bool validInput = false;
  while (not validInput) {
    cout << "Would you like to\n" <<
    "Enter 6 to Discard Card\n" <<
    "Enter 7 to keep hand\n";
    std::string input;
    cin >> input;
    if (input_helper::is_number(input)) {
      try {
        if (std::stod(input) == 7) {
          //Do nothing
          validInput = true;
        }
        else if (std::stod(input) == 6) {
          bool stop = false;
          std::vector<int> toRemove;
          cout << "Enter indexes of the cards to remove"
            " enter n to stop\n";
          while (not stop) {
            std::string removeMe;
            cin >> removeMe;
            if (not removeMe.compare("n")) {
              stop = true;
              validInput = true;
            }
            else if (input_helper::is_pos_number(removeMe)) {
              if (std::stod(removeMe) <= 0 or std::stod(removeMe) >= 5) {
                cerr << "must be between 1 and 5!" << endl;
              }
              else {
                toRemove.push_back(5-(std::stod(removeMe)));
              }
            }
            else {
              cerr << "Invalid input, please try again" << endl;
            }
          }
          table.commitPlayerAction(idx, 6, 0, toRemove);
          //Print new hand
          cout << table.getPlayerHand(idx) << endl;
        }
      }
      catch (std::exception e) {
        cerr << "Exception Caught! " << e.what() << endl; 
      }
    }
  }
}

int main(int argc, char const *argv[])
{
  PokerTable table = PokerTable(0);
  table.addPlayer(Player(100, "Player 1", true));
  table.addPlayer(Player(100, "Player 2", true));
  table.addPlayer(Player(100, "Player 3", true));
  table.addPlayer(Player(100, "Player 4", true));

  while (true) {
    bool gameOver = false;
    while (not gameOver) {
      //Round init, deal the cards
      switch (table.getCurrRound())
      {
        case Player::RoundCat::INIT: {
          table.dealHands();
          break;
        }
        case Player::RoundCat::BETTING1: {
          int size = table.getNumPlayers();
          for (int i = 0; i < size; ++i) {
            if (not table.isPlayerComputer(i)) {
              cout << table.getPlayerHand(i);
              if (not table.isOpened()) {
                handleNotOpenTable(table, i);
              }
              else {
                handleOpenTable(table, i);
              }
            }
            else {
              // cout << table.getPlayerHand(i);
              table.doComputerPlayerAction(i);
            }
          }
          break;
        }
        case Player::RoundCat::DRAW: {
          if (table.allPlayersFolded()) {
            cout << "Everyone Folded" << endl;
            gameOver = true;
            break;
          }
          int size = table.getNumPlayers();
          for (int i = 0; i < size; ++i) {
            if (not table.isPlayerFolded(i) and not table.isPlayerComputer(i)) {
              handleDrawPhase(table, i);
            }
            if (not table.isPlayerFolded(i) and table.isPlayerComputer(i)) {
              table.doComputerPlayerAction(i);
            }
          }
          break;
        }
        case Player::RoundCat::BETTING2: {
          if (table.allPlayersFolded()) {
            cout << "Everyone Folded" << endl;
            gameOver = true;
            break;
          }
          std::vector<int> players = table.getPlayerOrder();
          for (int i : players) {
            if (not table.isPlayerFolded(i) and not table.isPlayerComputer(i)) {
              cout << table.getPlayerHand(i);
              handleOpenTable(table, i);
            }
            if (not table.isPlayerFolded(i) and table.isPlayerComputer(i)) {
              table.doComputerPlayerAction(i);
            }
          }
          break;
        }
        case Player::RoundCat::SHOWDOWN: {
          if (table.allPlayersFolded()) {
            cout << "Everyone Folded" << endl;
            gameOver = true;
            break;
          }
          bool tied = false;
          Player winner = table.completeShowdown(tied);
          cout << winner.printHand();
          gameOver = true;
          break;
        }
        default: {
          cerr << "Round Category unknown!" << endl;
        }
      }
      table.advanceRound();
    }
    cout << "Would everyone like to go another round? y or n?" << endl;
    std::string input;
    cin >> input;
    if (not input.compare("n")) {
      break;
    }
    else {
      table.resetRound();
    }
  }
  return 0;
}
