#include"Event.h"
#include<vector>
#include<iomanip>
using namespace std;

int main(void) {
  std::vector<Event> A = {0.0, 0.25, 0.5, 0.75, 1.0, 0.3, 0.8, 0.45, 0.16};
  std::vector<Event> B = {0.0, 0.25, 0.5, 0.75, 1.0, 0.6, 0.12, 0.98, 0.33};
  for (int i = 0; i < A.size(); i++) {
    cout << "Testing Event # " << i << endl;
    cout << "P(A): " << A[i].toString() << endl;
    cout << "P(B): " << B[i].toString() << endl; 
    cout << "P(A and B)" << " | " << setw(5) 
        << "P(A or B)" << " | " << setw(5)
      << "P(~A and ~B)" << " | " << setw(5)
      << "P(A and ~B)" << endl;
    cout << (A[i] & B[i]) << setw(15);
    cout << (A[i] | B[i]) << setw(15);
    cout << (A[i] ^ B[i])  << setw(15);
    cout << (A[i] - B[i])  << setw(15);
    cout << endl;
  }
  return 0;
}