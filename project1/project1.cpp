#include<iostream>
#include "triangle.cpp"

using namespace std;
// @Brief - Checks if a string is a positive integer
// @param[in] s - string to evaluate
// @return bool true if string is a positive integer false otherwise
bool is_pos_num(const string s) {
  string::const_iterator it = s.begin();
  while (it != s.end() and isdigit(*it)) ++it;
  return not s.empty() and it == s.end();
}

int main() {
  int num_entries = 0;
  //Array to fill in
  array<unsigned int, 3> tri;
  //Keep prompting the user for valid input
  //Inform them if input is invalid and prompt again.
  do {
    string input;
    //Prompt the user for input
    cout << "Enter a positive integer length of side "
          << (num_entries+1) << " for the triangle\t";
    //Get line handles null input. cin >> input crashes the program...
    getline(std::cin, input);
    //Ensure that the input is valid
    if (is_pos_num(input))
      tri[num_entries++] = stoi(input);
    else 
      cout << "\n" << input << " is not a positive integer" << endl;
  } while (num_entries < 3);
  //Create the triangle
  triangle inTriangle(tri);
  //Print out what we want to know
  cout << "Triangle Stats: " << endl << inTriangle;

  return 0;
};