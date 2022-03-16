#include "Tree.h"

namespace TreeBuilder {
  std::vector<std::string> tokenize(std::string input) {
  std::vector<std::string> tokens;
    const int TOKEN = 1;
    const int SPACE = 0;
    int mode = SPACE;
    std::string token;
    for (char c : input) {
      if (c == ' ' or c == '\t' or c == '\n') {
        if (mode == TOKEN) {
          tokens.push_back(token);
          token.clear();
        }
        mode = SPACE;
      }
      else {
        token.push_back(c);
        mode = TOKEN;
      }
    }
    if (mode == TOKEN) {
      tokens.push_back(token);
    }

    return tokens;
  }

  bool isVar(std::string s) {
    return (s.front() >= 'a' and s.front() <= 'z') ||
            (s.front() >= 'A' and s.front() <= 'Z');
  }

  bool isNum(std::string s) {
    return s.front() >= '0' and s.front() <= '9';
  }

  bool isOperator(std::string s) {
    return s == "+" or s == "-" or s == "*" or s == "/";
  }

  bool isExpression(
  std::vector<std::string>::iterator first,
  std::vector<std::string>::iterator last) {
    if (first == last) {
      return false;
    }
    else if (last - first == 1) {
      return isNum(*first) or isVar(*first);
    }
    else if (*first == "(" and *(last - 1) == ")" and
            isExpression(first + 1, last - 1)) {
      return true;
    }
    else {
      for (std::vector<std::string>::iterator middle = first + 1;
          middle != last - 1; middle++) {
        if (isOperator(*middle) and isExpression(first, middle) and
        isExpression(middle+1, last)) {
          return true;
        }
      }
    }
    return false;
  }

  Tree* readInfix(
  std::vector<std::string>::iterator first,
  std::vector<std::string>::iterator last) {
    if (first == last) {
      return NULL;
    }
    else if (last - first == 1) {
      if (isVar(*first)) {
        return new Var(*first);
      }
      else if (isNum(*first)) {
        return new Constant(stod(*first));
      }
      else {
        return NULL;
      }
    }
    else if (*first == "(" and *(last - 1) == ")") {
      return Paren::construct(readInfix(first+1, last-1));
    }
    else {
      for (std::vector<std::string>::iterator middle = first + 1; 
          middle != last - 1; middle++) {
        Tree* l, *r;
        if (isOperator(*middle) and
        (l = readInfix(first, middle)) and
        (r = readInfix(middle+1, last))) {
          if (*middle == "+") {
            if (10 <= l->precedence() and 10 <= r->precedence()) {
              return Add::construct(l,r);
            }
          }
          else if (*middle == "-") {
            if (10 <= l->precedence() and 10 <= r->precedence()) {
              return Sub::construct(l, r);
            }
          }
          else if (*middle == "*") {
            if (50 <= l->precedence() and 50 <= r->precedence()) {
              return Mul::construct(l, r);
            }
          }
          else {
            if (50 <= l->precedence() and 50 <= r->precedence()) {
              return Div::construct(l, r);
            }
          }
        }
      }
    }
    return NULL;
  }

  Tree* buildTree(std::string s) {
    std::vector<std::string>  tokens = tokenize(s);
    return readInfix(tokens.begin(), tokens.end());
  }
}