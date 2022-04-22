#include "PokerTable.cpp"
#include "../helpers/input_helper.h"
using namespace std;
//@Brief - does the prompting availble for an open table
//@Param[inout] table - poker table
//@Param[in] i - player index
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
//@Brief - Handles the options for an unopened table
//@Param[inout] table - poker table
//@Param[in] i - seat of the current player
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
//@Brief - handles the draw phase of a poker game
//@Param[inout] table - poker table
//@Param[in] idx - seat of the active player
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
PokerTable setupTable() {
  string ante;
  PokerTable table;
  while (true) {
    cout << "What is the Ante for the Table? Cannot less than 0" << endl;
    cin >> ante;
    if (not input_helper::is_pos_number(ante)) {
      cout << "Please enter a positive number" << endl;
    }
    try {
      table.setTableAnte(stod(ante));
      break;
    } catch (out_of_range) {
      cerr << "Value out of range of given datatype" << endl;
    }
  }
  double totPlayers;
  while(true) {
    string numPlayers;
    cout << "How many players are in the game? Must be between 2 and 7" << endl;
    cin >> numPlayers;
    try {
      if (not input_helper::is_pos_number(numPlayers)) {
        cout << "Please enter a positive number" << endl;
      }
      else if (stod(numPlayers) < 2 or stod(numPlayers) > 7) {
        cout << "Please enter a number between 2 and 7" << endl;
      }
      else {
        totPlayers = stod(numPlayers);
        break;
      }
    } catch (out_of_range) {
      cerr << "Value out of range of given datatype" << endl;
    }
  }
  int i =0;
  while(i < totPlayers) {
    cout << "How much money does player" << (i+1) 
    << " have? Must be at least: " << ante << endl;
    string money;
    string computer;
    try {
      cin >> money;
      if (not input_helper::is_pos_number(money)) {
        cout << "Please enter a positive number" << endl;
      }
      else if (stod(money) < stod(ante)) {
        cout << "Please enter a value equal to or greater than the ante" 
        << endl;
      }
      else {
        cout << "Is Player" << (i+1) << " a computer? y or n" << endl;
        cin >> computer;
        if (not computer.compare("y")) {
          table.addPlayer(stod(money), "Player"+to_string(++i),true);
        }
        else {
          table.addPlayer(stod(money), "Player"+to_string(++i), false);
        }
      }
    } catch (out_of_range) {
      cerr << "Value out of range of given datatype" << endl;
    }
  }
  return table;
}
int main(int argc, char const *argv[])
{
  PokerTable table = setupTable();
  while (true) {
    bool gameOver = false;
    while (not gameOver) {
      switch (table.getCurrRound())
      {
        //Round init, deal the cards
        case Player::RoundCat::INIT: {
          table.dealHands();
          int size = table.getNumPlayers();
          for (int i = 0; i < size; ++i) {
            cout << table.getPlayerInfo(i);
          }
          break;
        }
        //Open with first player. Handled unopened and opened table
        case Player::RoundCat::BETTING1: {
          int size = table.getNumPlayers();
          int openIdx = 0;
          while (not table.isOpened()) {
            for (int i = 0; i < size; ++i) {
              if (not table.isPlayerComputer(i)) {
                cout << table.getPlayerHand(i);
                if (not table.isOpened()) {
                  handleNotOpenTable(table, i);
                }
              }
              else {
                table.doComputerPlayerAction(i);
              }
              if (table.isOpened()) { openIdx = i; }
            }
          }
          for (int i = 0; i < size; ++i) {
            if (i == openIdx) {continue;}
            if (not table.isPlayerComputer(i)) {
              cout << table.getPlayerHand(i);
              handleOpenTable(table, i);
            }
            else {
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
          int openIdx = 0;
          while (not table.isOpened()) {
            std::vector<int> players = table.getPlayerOrder();
            for (int i : players) {
              if (not table.isPlayerFolded(i) and not table.isPlayerComputer(i)) {
                cout << table.getPlayerHand(i);
                handleNotOpenTable(table, i);
              }
              if (not table.isPlayerFolded(i) and table.isPlayerComputer(i)) {
                table.doComputerPlayerAction(i);
              }
              if (table.isOpened()) {openIdx = i;}
            }
          }
          std::vector<int> players = table.getPlayerOrder();
          for (int i : players) {
            if (openIdx == i) {continue;}
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
          std::vector<Player> winner = table.completeShowdown(tied);
          if (not tied) {
            cout << "\nWinner! " << winner[0].printHand() << endl;
          }
          else {
            cout << There was a tie! << endl;
            for (const Player & player : winner) {
              cout << "\nWinner! " << winner[0].printHand() << endl;
            }
          }
          cout << "All Hands this round: \n" << table.showPlayerHands();
          gameOver = true;
          table.adjustPlayerMoney();
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
      cout << "Some Players may have been removed for not being able to make the "
        "minimum ante" << endl;
      if (table.getNumPlayers() < 2) {
        cout << "Not Enough Players left to play!" << endl;
      }
    }
  }
  return 0;
}
