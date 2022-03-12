#include"Event.h"
#include<vector>
#include<iomanip>
using namespace std;

int main(void) {
  std::vector<Event> A = {0.0, 0.25, 0.5, 0.75, 1.0, 0.3, 0.8, 0.45, 0.16};
  std::vector<Event> B = {0.0, 0.25, 0.5, 0.75, 1.0, 0.6, 0.12, 0.98, 0.33};
  for (int i = 0; i < A.size(); i++) {
    cout << "Testing Events # " << i << endl;
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

  Event temp = A[1];
  Event temp1 = B[2];
  cout << "&= Test" << endl;
  cout << "A: " << temp.toString() << endl;
  cout << "B: " << temp1.toString() << endl;
  cout << "P(A &= B) 3 times" << endl;
  for (int i = 0; i < 3; i++) {
    temp &= temp1;
  }
  cout << temp.toString() << endl;

  Event temp2 = A[5];
  Event temp3 = B[6];
  cout << "|= Test" << endl;
  cout << "A: " << temp2.toString() << endl;
  cout << "B: " << temp3.toString() << endl;
  cout << "P(A |= B) 3 times" << endl;
  for (int i = 0; i < 3; i++) {
    temp2 &= temp3;
  }
  cout << temp2.toString() << endl;

  Event temp4 = A[6];
  Event temp5 = B[2];
  cout << "|= Test" << endl;
  cout << "A: " << temp4.toString() << endl;
  cout << "B: " << temp5.toString() << endl;
  cout << "P(A ^= B) 3 times" << endl;
  for (int i = 0; i < 3; i++) {
    temp4 &= temp5;
  }
  cout << temp4.toString() << endl;

  Event temp6 = A[7];
  Event temp7 = B[1];
  cout << "|= Test" << endl;
  cout << "A: " << temp6.toString() << endl;
  cout << "B: " << temp7.toString() << endl;
  cout << "P(A ^= B) 3 times" << endl;
  for (int i = 0; i < 3; i++) {
    temp6 &= temp7;
  }
  cout << temp6.toString() << endl;

  return 0;
}