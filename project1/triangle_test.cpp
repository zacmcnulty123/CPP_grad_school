#include "triangle.cpp"
#include <assert.h>
using namespace std;

//Small method that test the functions of the triangle class
int main (void) {
  cout << "Scalene Test 1" << endl;
  triangle test_triangle(array<unsigned int,3>({1,2,3}));
  string test1 = test_triangle.get_triangle_type();
  //compare return 0 if the strings match and assert fails
  //When the return value is 0 so this has to be inverse logic...
  assert(not test1.compare("Scalene"));
  cout << "Pass" << endl;
  cout << "Isosceles Test" << endl;
  triangle test_triangle2(array<unsigned int,3>({2,2,3}));
  string test2 = test_triangle2.get_triangle_type();
  cout << "Pass" << endl;
  assert(not test2.compare("Isosceles"));
  cout << "Equilateral Test" << endl;
  triangle test_triangle3(array<unsigned int,3>({2,2,2}));
  string test3 = test_triangle3.get_triangle_type();
  assert(not test3.compare("Equilateral"));
  cout << "Pass" << endl;
};