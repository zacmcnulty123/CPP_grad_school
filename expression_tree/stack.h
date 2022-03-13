#pragma once
#include"node.h"

template<typename T>
class Stack {
  Node<T> *node;
  Stack *next;
  Stack(Node<T> *node) {
    this->node = node;
    next = NULL;
  }
};