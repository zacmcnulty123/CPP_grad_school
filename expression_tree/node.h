#pragma once
#include<string>

typedef double consta;
typedef std::string var;
typedef std::string op;

// template<typename T>
class Node {
private:
public:
  std::string val;
  Node  *left, *right;
  Node(std::string rhs) {
    val = rhs;
    left = NULL;
    right = NULL;
  }

  Node() {
    left = NULL;
    right = NULL;
  }

  Node operator =(Node rhs) {
    Node n;
    n.val = rhs.val;
    n.left = rhs.left;
    n.right = rhs.right;
  }

  bool isLeaf(Node* node) {
    return node->left == nullptr and node->right == nullptr;
  }
};