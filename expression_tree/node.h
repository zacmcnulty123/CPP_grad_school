#pragma once
#include<string>
#include<map>

//The node can be a variable of a constant
typedef double constant;
typedef std::string variable; 
typedef std::string op;

class Node {
private:
  /* data */
  std::string symbol;
public:
  Node *left;
  Node *right;

  Node(std::string data) {
    this->symbol = data;
    left = NULL;
    right = NULL;
  }
  ~Node();

  void setLeft(Node *lhsRoot) {
    left = lhsRoot;
  }

  void setRight(Node *rhsRoot) {
    right = rhsRoot;
  }

  std::string getSymbol() const {
    return this->symbol;
  }
};
