#include"Event.h"

using namespace std;
int main(void) {
  Event A = Event(0.5);
  Event B = Event(0.5);
  cout << (A & B) << endl;
  cout << (A | B) << endl;
  cout << (A ^ B) << endl;
  cout << (A - B) << endl;
  return 0;
}