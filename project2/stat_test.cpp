#include "statistics.cpp"
#include "input_helper.h"
#include "assert.h"
using namespace std;


// Test to ensure that a single length list works
bool single_length_array_test() {
  cout << __func__ << endl;
  std::string filename = "./tests/single.csv";
  statistics<double> myStats;
  std::vector<double> csv_content;
  if (not input_helper::parse_csv(filename, csv_content)) {
    cout << "Something went wrong parsing the csv!" << endl;
    return false;
  }
  myStats = statistics<double>(csv_content);
  cout << "Entered Values: " << myStats << endl;
  cout << myStats.get_descriptive_statistics() << endl;
  return true;
}

// Test that a sequence of length 2 can be used
bool size_two_length_array_test() {
  cout << __func__ << endl;
  std::string filename = "./tests/two.csv";
  statistics<double> myStats;
  std::vector<double> csv_content;
  if (not input_helper::parse_csv(filename, csv_content)) {
    cout << "Something went wrong parsing the csv!" << endl;
    return false;
  }
  myStats = statistics<double>(csv_content);
  cout << "Entered Values: " << myStats << endl;
  cout << myStats.get_descriptive_statistics() << endl;
  return true;
}

// Test that odd sized sequences work
bool odd_size_seq_test() {
  cout << __func__ << endl;
  std::string filename = "./tests/odd.csv";
  statistics<double> myStats;
  std::vector<double> csv_content;
  if (not input_helper::parse_csv(filename, csv_content)) {
    cout << "Something went wrong parsing the csv!" << endl;
    return false;
  }
  myStats = statistics<double>(csv_content);
  cout << "Entered Values: " << myStats << endl;
  cout << myStats.get_descriptive_statistics() << endl;
  return true;
}

// Test that even sized sequenced work
bool even_size_seq_test() {
  cout << __func__ << endl;
  std::string filename = "./tests/even.csv";
  statistics<double> myStats;
  std::vector<double> csv_content;
  if (not input_helper::parse_csv(filename, csv_content)) {
    cout << "Something went wrong parsing the csv!" << endl;
    return false;
  }
  myStats = statistics<double>(csv_content);
  cout << "Entered Values: " << myStats << endl;
  cout << myStats.get_descriptive_statistics() << endl;
  return true;
}

int main (void) {
  assert(single_length_array_test());
  assert(size_two_length_array_test()); 
  assert(odd_size_seq_test());
  assert(even_size_seq_test());
  return 0;
};