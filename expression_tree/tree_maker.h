#include"tree.h"

Tree Add(Tree * lhs, Tree *rhs) {
  Node n("+");
  Tree t(&n);
  t.getRoot()->setLeft(lhs->getRoot());
  t.getRoot()->setRight(rhs->getRoot());
  return t;
}

Tree Constant(double value) {
  Node n(std::to_string(value));
  Tree t(&n);
  t.getRoot()->left = NULL;
  t.getRoot()->right = NULL;
  return t;
}

Tree Variable(std::string var) {
  Node n(var);
  Tree t(&n);
  t.getRoot()->left = NULL;
  t.getRoot()->right = NULL;
  return t;
}