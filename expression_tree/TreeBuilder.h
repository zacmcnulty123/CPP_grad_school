#include "Tree.h"
namespace TreeBuilder {
  //@Brief - Tokenize a string to create an expression tree from
  //@Param[in] input - String to tokenize
  //@return std::vector<std::string> tokens
  std::vector<std::string> tokenize(std::string input) {
    std::vector<std::string> tokens;
    //string steam to delimit
    std::stringstream check1(input);

    std::string intermediate;
    //Tokenize based on spaces
    while(getline(check1, intermediate, ' '))
    {
      tokens.push_back(intermediate);
    }
    return tokens;
  }

  //@Brief - Determines if token is a variable
  //@Param[in] token - token to evaluate
  //@return bool
  bool isVar(std::string token) {
    return (token.front() >= 'a' and token.front() <= 'z') ||
            (token.front() >= 'A' and token.front() <= 'Z');
  }

  //@Brief - Determines if token is a constant
  //@Param[in] token - token to evaluate
  //@return bool
  bool isNum(std::string token) {
    return token.front() >= '0' and token.front() <= '9';
  }

  //@Brief - Determines if token is an operator
  //@Param[in] token - token to evaluate
  //@return bool
  bool isOperator(std::string s) {
    return s == "+" or s == "-" or s == "*" or s == "/";
  }

  //@Brief - Reads an infix string expression and
  // converts that to an expression tree
  //@Param[in] first : pointer to the first token
  //@Param[out] last : Pointer to the last token
  //@return Tree*
  Tree* readInfix(
  std::vector<std::string>::iterator first,
  std::vector<std::string>::iterator last) {
    //Can't make a tree out of a single token
    if (first == last) {
      return NULL;
    }
    //Must be a var or a number
    else if (last - first == 1) {
      if (isVar(*first)) {
        return new Var(*first);
      }
      else if (isNum(*first)) {
        return new Constant(stod(*first));
      }
      else {
        //Return NULL if invalid
        return NULL;
      }
    }
    else if (*first == "(" and *(last - 1) == ")") {
      //Handle the inner of a parenthesis
      return Paren::construct(readInfix(first+1, last-1));
    }
    else {
      //Handle the operator case
      for (std::vector<std::string>::iterator middle = first + 1; 
          middle != last - 1; middle++) {
        Tree* l, *r;
        
        if (isOperator(*middle) and
        (l = readInfix(first, middle)) and
        (r = readInfix(middle+1, last))) {
          if (*middle == "+") {
            if (2 <= l->precedence() and 2 <= r->precedence()) {
              return Add::construct(l,r);
            }
          }
          else if (*middle == "-") {
            if (2 <= l->precedence() and 2 <= r->precedence()) {
              return Sub::construct(l, r);
            }
          }
          else if (*middle == "*") {
            if (3 <= l->precedence() and 3 <= r->precedence()) {
              return Mul::construct(l, r);
            }
          }
          else {
            if (3 <= l->precedence() and 3 <= r->precedence()) {
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