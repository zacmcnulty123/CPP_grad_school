#include<array>
#include<unordered_map>
#include<algorithm>
#include<iterator>
#include<iostream>
#include<sstream>
class triangle {
private:
    //@Brief - Returns the number of sides with the same length
    // for a triangle
    //@return unsigned int: Number of matching sides for the triangle
    unsigned int get_num_matching_sides() const {
      //Using a map like this is just a quick way to count
      //the frequency of a given value in a list
      std::unordered_map<unsigned int, size_t> count;
      for (const auto elem : this->sides) {
        count[elem]++;
      }
      //Find the largest value in the map
      unsigned int max = 0;
      for (const auto pair : count) {
        if (pair.second > max) {
          max = pair.second;
        }
      }
      //Return the max
      return max;
    }

public:
    //Member variable for the lengths of the triangle
    //Was a bit lazy here by not writing a setter and getter
    //and making this a private member variable
    std::array<unsigned int, 3> sides;

    //@Brief - parameterized class constructor
    triangle(const std::array<unsigned int, 3> sides_) {
      sides = sides_;
    }

    //@Brief - Returns what type of triangle the triangle is
    //@return string - String with the name of the type of triangle
    std::string get_triangle_type() const {
      std::string ret;
      switch (this->get_num_matching_sides()) {
        case 1:
          ret = "Scalene";
          break;
        case 2:
          ret = "Isosceles";
          break;
        default:
          ret = "Equilateral";
      }
      return ret;
    }

    std::string to_string() const { 
      std::stringstream out;
      int i = 1;
      for (const auto elem : this->sides) {
        out << "Side: " << std::to_string(i++) << " length: "
        << std::to_string(elem) << std::endl;
      }
      out << "This Triangle is "
      << this->get_triangle_type() << std::endl;
      return out.str();
    }
};

//Overloads the stream operator to print out some useful
// information about the class
std::ostream & operator<< (
  std::ostream &out,
  const triangle & tri) {
  out << tri.to_string();
  return out;
}