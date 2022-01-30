#include "statistics.cpp"

using namespace std;

int main (void) {
  //Average Test
  statistics myStats = statistics();

  for (double i = 1.0; i < 20.0; i++) {
    myStats.enqueue(i);
  }
  cout.precision(2);
  cout << "Average: " << fixed << myStats.get_mean() << endl;

  cout << myStats << endl;

  double arr[5] = {600, 470, 170, 430, 300};
  statistics myStats1 = statistics();
  for (int i = 0; i < 5; i++) {
    myStats1.enqueue(arr[i]);
  }

  cout << "Variance: " << myStats1.get_variance() << endl;
  cout << "STD: " << myStats1.get_STD() << endl;
};