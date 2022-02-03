#include "statistics.cpp"
#include "input_helper.h"
using namespace std;

int main (void) {

  std::string filename = "myCsv.csv";
  statistics<double> myStats;

  std::vector<double> csv_content;

  if (not input_helper::parse_csv(filename, csv_content)) {
    cout << "Something went wrong parsing the csv!" << endl;
  }
  myStats = statistics<double>(csv_content);
  cout << myStats << endl;
  cout << "Mode: " << myStats.get_mode() << endl;
  cout.precision(2);
  cout << "Average: " << fixed << myStats.get_mean() << endl;
  cout << "Variance: " << myStats.get_variance() << endl;
  cout << "STD: " << myStats.get_STD() << endl;
  cout << "Quartiles\n" << myStats.get_quartiles() << endl;
  // statistics<string> myStats2 = statistics<string>();
};