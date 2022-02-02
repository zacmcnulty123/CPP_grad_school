#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<stdexcept>

namespace input_helper {

  bool is_pos_number(const std::string s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() and isdigit(*it)) ++it;
    return not s.empty() and it == s.end();
  }

  template<typename T> 
  bool parse_csv(std::string filename, std::vector<T> & out) {
    std::ifstream file_to_parse(filename);
    if (not file_to_parse.is_open()) {
      throw std::runtime_error("Could not open file");
      return false;
    }
    if (not file_to_parse.good()) {
      return false;
    }
    std::string line;
    T val;
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
    file_to_parse.close();
    return true;
  }
};