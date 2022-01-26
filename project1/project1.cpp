#include<iostream>
#include<string>
#include<array>
#include<unordered_map>
const unsigned int NUM_SIDES = 3;
using namespace std;


bool is_pos_num(const string s) {
  string::const_iterator it = s.begin();
  while (it != s.end() and isdigit(*it)) ++it;
  return !s.empty() and it == s.end();
}

unsigned int get_num_array_els_equal(
  const std::array<unsigned int, NUM_SIDES> arr) {
  std::unordered_map<unsigned int, size_t> count;
  for (const auto elem : arr) {
    count[elem]++;
  }
  unsigned int max = 0;
  for (const auto pair : count) {
    if (pair.second > max)
    {
      max = pair.second;
    }
  }
  return max; 
}

int main() {
  int num_entries = 0;
  std::array<unsigned int, NUM_SIDES> tri;
  do {
    string input;
    cout << "Enter a non-negative integer length of side "
          << (num_entries+1) << " for the triangle\t";
    getline(std::cin, input);
    if (is_pos_num(input)) 
      tri[num_entries++] = stoi(input);
    else
      cout << "\n" << input << " is not a non-negative integer" << endl;
  } while (num_entries<NUM_SIDES);
  switch (get_num_array_els_equal(tri)) {
    case 1:
      cout << "You entered a scalene triangle!" << endl;
      break;
    case 2:
      cout << "You entered an isosceles triangle!" << endl;
      break;
    default:
      cout << "You entered an equilateral triangle!" << endl;
  }
};