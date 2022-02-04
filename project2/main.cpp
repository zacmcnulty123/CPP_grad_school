#include "input_helper.h"
#include "statistics.cpp"
using namespace std;
int main (void) {
  statistics<double> stats = statistics<double>();
  while (true) {
    string input;
    //Prompt user for input
    cout << "Enter a number to add the the sequence or enter stop: "
      << endl;
    cin >> input;
    //covert the string to lowercase for easy comparison
    std::for_each(input.begin(), input.end(), [](char & c) {
        c = ::tolower(c);
    });
    //check termination condition
    if (not input.compare("stop")) {
      break;
    } 
    else if (input_helper::is_number(input)) {
      //Want to catch values that are too large to contain in the current datatype
      try {
        //Add the value to the sequence
        stats.enqueue(stoi(input));
      } catch (std::out_of_range) {
        cout << "Value out of range for given datatype" << endl;
      }
    }
    else {
      //Inform the user that their input is invalid
      cout << input << " is not a number!" << endl;
    }
  }
  cout << "\nEntered Values: " << stats << endl;
  //Print out the information that we're looking for
  cout << "Descriptive Statistics: \n" << stats.get_descriptive_statistics() << endl;
  return 0;
};