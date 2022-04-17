#include "PokerTable.h"
#include "../helpers/input_helper.h"
using namespace std;

void handleOpenTable(PokerTable & table, const int & i) {
  bool validInput = false;
  while (not validInput) {
    cout << "You currently have: " << table.getPlayerMoney(i)
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
  table.addPlayer(Player(100, "Player 1"));
  table.addPlayer(Player(100, "Player 2"));

  bool gameOver = false;

  while (not gameOver) {
    //Round init, deal the cards
    switch (table.getCurrRound())
    {
      case PokerTable::RoundCat::INIT: {
        table.dealHands();
        break;
      }
      case PokerTable::RoundCat::BETTING1: {
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
        }
        break;
      }
      case PokerTable::RoundCat::DRAW: {
        int size = table.getNumPlayers();
        for (int i = 0; i < size; ++i) {
          handleDrawPhase(table, i);
        }
        gameOver = true;
        break;
      }
      case PokerTable::RoundCat::BETTING2: {
        break;
      }
      case PokerTable::RoundCat::SHOWDOWN: {
        break;
      }
      default: {
        cerr << "Round Category unknown!" << endl;
      }
    }
    table.advanceRound();
  }
  return 0;
}
