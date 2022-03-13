#pragma once
#include"node.h"
// #include"stack.h"
#include<string>
// template<typename T>
class Tree
{
private:
  /* data */
  //Create a stack pointer to the top of the tree
  Node *root;
public:
  Tree(Node * node) {
    root->left = node->left;
    root->right = node->right;
    }
  ~Tree() {};

  Tree operator =(Tree rhs) {
    Tree t(rhs.getRoot());
    this->root->left = rhs.getRoot()->left;
    this->root->right = rhs.getRoot()->right;
    return t;
  }
  Node* getRoot() {
    return this->root;
  }
};