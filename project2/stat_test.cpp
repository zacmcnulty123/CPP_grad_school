#include "statistics.cpp"

using namespace std;

int main (void) {
  //Average Test
  statistics<double> myStats = statistics<double>();

  cout << "Mode: " << myStats.get_mode() << endl;
  for (double i = 1.0; i < 20.0; i++) {
    myStats.enqueue(i);
  }
  myStats.enqueue(2);
  myStats.enqueue(2);
  cout.precision(2);
  cout << "Average: " << fixed << myStats.get_mean() << endl;
  cout << myStats << endl;

  double arr[5] = {600, 470, 170, 430, 300};
  statistics<double> myStats1 = statistics<double>();
  for (int i = 0; i < 5; i++) {
    myStats1.enqueue(arr[i]);
  }

  cout << "Variance: " << myStats1.get_variance() << endl;
  cout << "STD: " << myStats1.get_STD() << endl;
};