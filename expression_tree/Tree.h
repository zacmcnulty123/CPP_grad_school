#pragma once
#include <sstream>
#include <string>
#include <stack>
#include <map>
#include <iostream>
#include <vector>

class Tree
{
private:
  /* data */
public:
  //@Brief - Prints out the contents of the Tree
  //@Return std::string
  virtual std::string toString() const = 0;

  //@Brief - Evaluates the current level of the tree
  //@Param[in] vars - Map of variable names to their respective values
  //@Return double - Value of the evaluation
  virtual double eval(const std::map<std::string, double> vars) const = 0;

  //@Brief - Creates the derivative of the current sub tree
  //@Param[in] var - Name of the variable to take the derivative
  // with respect to
  //@Return Tree* - Tree structure
  virtual Tree* derive(const std::string var) const = 0;

  virtual int precedence() const = 0;

  Tree() {}
  ~Tree(){}
};

class Constant : public Tree {
private:
  //@Brief - Gets the value of the constant
  //@Return double
  const double getValue() const {
    return this->value;
  }
public:
  virtual std::string toString() const {
    std::stringstream ss;
    ss << value;
    return ss.str();
  }
  virtual double eval(const std::map<std::string, double> /*vars*/) const {
    //Constants just return their value
    return this->value;
  }
  virtual Tree* derive(const std::string /*var*/) const {
    //Derived constants are always 0
    return new Constant(0.0);
  }

  virtual int precedence() const {
    return 100;
  }
  //Constant's value
  const double value;
  //Constructor for value
  Constant(double val) : value(val) {}
  //Copy constructor for cloning nodes
  Constant(Constant* root) : value(root->getValue()) {}
  ~Constant(){}
};

class Var : public Tree {
private:
  //@Brief - Returns the variable name
  //@Return std::string
  std::string getVar() const {
    return this->var;
  }
public:
  virtual std::string toString() const {
    return var;
  }
  virtual double eval(const std::map<std::string, double> vars) const {
    //Return the value that is given from the var -> value map
    return vars.at(this->var);
  }
  virtual Tree* derive(const std::string var) const {
    //If this is the variable we are deriving return 1.0
    //Otherwise return 0.0
    if (this->var.compare(var) == 0) {
      return new Constant(1.0);
    }
    else {
      return new Constant(0.0);
    } 
  }
  virtual int precedence() const {
    return 100;
  }
  //Name of the variables
  const std::string var;
  //Constructor
  Var(std::string val) : var(val) {}
  //Copy Constructor
  Var(Var* root) : var(root->getVar()) {}
  ~Var(){}
};

class Paren : public Tree {
private:
  //@Brief - Return the subtree that the Parens encapsulates
  //@Return Tree*
  const Tree* getInner() const {
    return this->inner;
  }
public:
  virtual std::string toString() const {
    std::stringstream ss;
    ss << "(";
    ss << inner->toString();
    ss << ")";
    return ss.str();
  }
  virtual double eval(const std::map<std::string, double> vars) const {
    //Evaluate the inside of the parens
    return inner->eval(vars);
  }
  virtual Tree* derive(const std::string var) const {
    //Derive the inside of the parens
    return inner->derive(var);
  }

  virtual int precedence() const {
    return 100;
  }
  //subtree that the parens encapsulate
  const Tree* inner;
  //Constructor
  Paren(const Tree* exp) : inner(exp) {}
  //Copy constructor for cloning
  Paren(const Paren* root) : inner(root->getInner()){}

  static Tree* construct(const Tree* exp) {
    return exp ? new Paren(exp) : NULL;
  }
  //Need to handle pointers
  ~Paren() {inner = NULL;}
};

class Add : public Tree {
private:
  //@Brief - Returns the left subtree of the +
  //@Return Tree*
  const Tree* getLeft() const {
    return this->left;
  }

  //@Brief - Returns the right subtree of the +
  //@Return Tree*
  const Tree* getRight() const {
    return this->right;
  }
public:
  virtual std::string toString() const {
    std::stringstream ss;
    ss << left->toString();
    ss << " + ";
    ss << right->toString();
    return ss.str();
  }

  virtual double eval(const std::map<std::string, double> vars) const {
    //L + R
    double leftVal = left->eval(vars);
    double rightVal = right->eval(vars);
    return leftVal + rightVal;
  }

  virtual Tree* derive(const std::string var) const {
    //L' + R'
    const Tree* expLeft(left);
    const Tree* expRight(right);
    return new Add(expLeft->derive(var), expRight->derive(var));
  }
  virtual int precedence() const {
    return 10;
  }
  //Left subtree
  const Tree* left;
  //Right subtree
  const Tree* right;
  //constructor
  Add(const Tree* lhs, const Tree* rhs) :
    left(lhs), right(rhs) {}
  //copy constructor
  Add(const Add* root) :
    left(root->getLeft()),
    right(root->getRight()) {}

  static Tree* construct(const Tree* lhs, const Tree* rhs) {
    return lhs and rhs ? new Add(lhs, rhs) : NULL;
  }
  ~Add() {left = NULL; right = NULL;}
};

class Sub : public Tree {
private:
  //@Brief - Returns the left subtree of the +
  //@Return Tree*
  const Tree* getLeft() const {
    return this->left;
  }

  //@Brief - Returns the right subtree of the +
  //@Return Tree*
  const Tree* getRight() const {
    return this->right;
  }
public:
  virtual std::string toString() const {
    std::stringstream ss;
    ss << left->toString();
    ss << " - ";
    ss << right->toString();
    return ss.str();
  }

  virtual double eval(const std::map<std::string, double> vars) const {
    //L - R
    double leftVal = left->eval(vars);
    double rightVal = right->eval(vars);
    return leftVal - rightVal;
  }

  virtual Tree* derive(const std::string var) const {
    //L' - R'
    const Tree* expLeft(left);
    const Tree* expRight(right);
    return new Sub(expLeft->derive(var), expRight->derive(var));
  }
  virtual int precedence() const {
    return 10;
  }

  const Tree* left;
  const Tree* right;
  Sub(const Tree* lhs, const Tree* rhs) :
    left(lhs), right(rhs) {}

  //copy constructor
  Sub(const Sub* root) :
    left(root->getLeft()),
    right(root->getRight()) {}

  static Tree* construct(const Tree* lhs, const Tree* rhs) {
    return lhs and rhs ? new Sub(lhs, rhs) : NULL;
  }
  ~Sub() {left = NULL; right = NULL;}
};

class Mul : public Tree {
private:
  //@Brief - Returns the left subtree of the +
  //@Return Tree*
  const Tree* getLeft() const {
    return this->left;
  }

  //@Brief - Returns the right subtree of the +
  //@Return Tree*
  const Tree* getRight() const {
    return this->right;
  }
public:
  virtual std::string toString() const {
    std::stringstream ss;
    ss << left->toString();
    ss << " * ";
    ss << right->toString();
    return ss.str();
  }

  virtual double eval(const std::map<std::string, double> vars) const {
    //L * R
    double leftVal = left->eval(vars);
    double rightVal = right->eval(vars);
    return leftVal * rightVal;
  }

  virtual Tree* derive(const std::string var) const {
    //L*R' + R*L'
    const Tree* expLeft(left);
    const Tree* expRight(right);
    return new Add(new Mul(expLeft, expRight->derive(var)),
                  new Mul(expRight, expLeft->derive(var)));
  }

  virtual int precedence() const {
    return 50;
  }

  const Tree* left;
  const Tree* right;
  Mul(const Tree* lhs, const Tree* rhs) :
    left(lhs), right(rhs) {}

  //copy constructor
  Mul(const Mul* root) :
    left(root->getLeft()),
    right(root->getRight()) {}

  static Tree* construct(const Tree* lhs, const Tree* rhs) {
    return lhs and rhs ? new Mul(lhs, rhs) : NULL;
  }
  ~Mul() {left = NULL; right = NULL;}
};

class Div : public Tree {
private:
  //@Brief - Returns the left subtree of the +
  //@Return Tree*
  const Tree* getLeft() const {
    return this->left;
  }

  //@Brief - Returns the right subtree of the +
  //@Return Tree*
  const Tree* getRight() const {
    return this->right;
  }
public:
  virtual std::string toString() const {
    std::stringstream ss;
    ss << left->toString();
    ss << " / ";
    ss << right->toString();
    return ss.str();
  }

  virtual double eval(const std::map<std::string, double> vars) const {
    //L / R -> Prevent divide by 0
    double leftVal = left->eval(vars);
    double rightVal = right->eval(vars);
    double ret = 0;
    if (rightVal != 0) {
      ret = leftVal / rightVal;
    }
    else {
      throw std::invalid_argument("Cannot Divide by 0!");
    }
    return ret;
  }

  virtual Tree* derive(const std::string var) const {
    //(R*L' - L * R')/ (R*R)
    const Tree* expLeft(left);
    const Tree* expRight(right);
    return new Div(new Paren(new Sub(new Mul(expRight, expLeft->derive(var)),
                    new Mul(expLeft, expRight->derive(var)))),
                    new Paren(new Mul(expRight, expRight)));
  }

  virtual int precedence() const {
    return 50;
  }

  const Tree* left;
  const Tree* right;
  Div(const Tree* lhs, const Tree* rhs) :
    left(lhs), right(rhs) {}

  //copy constructor
  Div(const Div* root) :
    left(root->getLeft()),
    right(root->getRight()) {}
  ~Div() {left = NULL; right = NULL;}

  static Tree* construct(const Tree* lhs, const Tree* rhs) {
    return lhs and rhs ? new Div(lhs, rhs) : NULL;
  }
};