#pragma once
#include<map>
#include<vector>
#include<iostream>
#include<sstream>
#include<stack>
#include"node.h"
class Tree
{
private:

  void toString (
    const Node *ptr,
    std::stringstream & ss) const {
    if (not ptr) return;
    toString(ptr->left, ss);

    ss << ptr->getSymbol();

    toString(ptr->right, ss);
  }

public:
  Node *root;
  std::map<std::string, double> varToValMap;
  Tree(/* args */);
  ~Tree();

  Tree operator =(Node * root) {
    Tree t;
    t.root = root;
    return t;
  }

  std::string toString(const Node *ptr) const {
    std::stringstream ss;
    toString(ptr, ss);
    return ss.str();
  }

  Node* getRoot() const {
    return this->root;
  }

  Tree Add(const Tree * lhs, const Tree * rhs) {
    Node rt("+");
    rt.setLeft(lhs->root);
    rt.setRight(rhs->root);
    Tree t;
    t.root = root;
    return t;
  }

  template<typename T, typename U>
  Tree Add(T * lhs, U * rhs) {
    std::cout << *lhs;
    Tree t;
    return t;
  }

  // Tree Mul() {

  // }

  // Tree Sub() {
  
  // }

  // Tree div() {

  // }
};

std::ostream &operator <<(std::ostream & out, const Tree t) {
  out << t.toString(t.getRoot());
  return out;
}

Tree::Tree(/* args */)
{
  root = NULL;
}

Tree::~Tree()
{
}
