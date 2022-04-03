#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<stdexcept>

//Helper library used for test harnesses.
namespace input_helper {

  //@Brief - Checks is string is a positive number
  //@Param[in] s - string to verify against
  //@return bool - true is string is a positive number; false otherwise
  bool is_pos_number(const std::string s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() and isdigit(*it)) ++it;
    return not s.empty() and it == s.end();
  }

  //@Brief - Checks if the input string is numeric
  //@Param[in] s - string to verify against
  //@return bool - true if string is numeric; false otherwise.
  bool is_number(const std::string s) {
    //first character will be - if negative
    if (s[0] == '-') {
      return is_pos_number(s.substr(1, s.size()));
    }
    else {
      return is_pos_number(s);
    }
  }

  //@Brief - Converts a string to lowercase
  //@Param[inout] s - String to convert to lowercase
  //@Return void
  void to_lower(std::string & s) {
    //covert the string to lowercase for easy comparison
    std::for_each(s.begin(), s.end(), [](char & c) {
        c = ::tolower(c);
    });
  }

  //@Brief - Parses CSV values into a vector
  //@Param[in] filename - name of the file
  //@Param[in] out - vector to return
  //@return bool - True if file parsed successfully; false otherwise
  template<typename T> 
  bool parse_csv(std::string filename, std::vector<T> & out) {
    std::ifstream file_to_parse(filename);
    //Don't try to parse an open file
    if (not file_to_parse.is_open()) {
      throw std::runtime_error("Could not open file: " + filename);
      return false;
    }
    //Make sure the file is good too parse
    if (not file_to_parse.good()) {
      return false;
    }
    std::string line;
    T val;
    //Parse the file
    try {
      while(std::getline(file_to_parse, line)) {
        std::stringstream ss(line);
        int col_idx = 0;
        while (ss >> val) {
          out.push_back(val);
          if (ss.peek() == ',')
            ss.ignore();
          col_idx++;
        }
      }
    }
    catch (const Exception & e) {
      std::stringstream msg;
      msg << "Error parsing file: " << e.what();
      throw std::runtime_error(msg.str());
    }
    //Close the file
    file_to_parse.close();
    return true;
  }
};