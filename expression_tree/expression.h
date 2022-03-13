#pragma once
#include<sstream>
#include<string>
#include<map>
#include<iostream>
class Expression
{
private:
  /* data */
public:
  virtual std::string toString() const = 0;
  virtual double eval(const std::map<std::string, double> vars) const = 0;
  virtual Expression* derive(const std::string var) const = 0;
  Expression() {}
  ~Expression(){}
};

class ExpressionConst : public Expression {
private:
double getValue() const {
  return this->value;
}
public:
  virtual std::string toString() const {
    std::stringstream ss;
    ss << value;
    return ss.str();
  }
  virtual double eval(const std::map<std::string, double> /*vars*/) const {
    return this->value;
  }
  virtual Expression* derive(const std::string /*var*/) const {
    return new ExpressionConst(0.0);
  }
  const double value;
  ExpressionConst(double val) : value(val) {}
  ExpressionConst(ExpressionConst* root) : value(root->getValue()) {}
  ~ExpressionConst(){}
};

class ExpressionVar : public Expression {
private:
std::string getVar() const {
  return this->var;
}
public:
  virtual std::string toString() const {
    return var;
  }
  virtual double eval(const std::map<std::string, double> vars) const {
    return vars.at(this->var);
  }
  virtual Expression* derive(const std::string var) const {
    if (this->var.compare(var) == 0) {
      return new ExpressionConst(1.0);
    }
    else {
      return new ExpressionConst(0.0);
    } 
  }
  const std::string var;
  ExpressionVar(std::string val) : var(val) {}
  ExpressionVar(ExpressionVar* root) : var(root->getVar()) {}
  ~ExpressionVar(){}
};

// class ExpressionMinus : public Expression {
//   public:
//     virtual std::string toString() const {
//       std::stringstream ss;
//       ss << "-";
//       ss << inner->toString();
//       return ss.str();
//     }
//     virtual double eval(const std::map<std::string, double> vars) const {
//       return -1 * (inner->eval(vars));
//     }
//     virtual Expression* derive(std::string var) const {

//     }
//     const Expression* inner;
//     ExpressionMinus(Expression* exp) : inner(exp) {}
//     ~ExpressionMinus() {inner = NULL;}
// };

class ExpressionParen : public Expression {
private:
const Expression* getInner() const {
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
    return inner->eval(vars);
  }
  virtual Expression* derive(const std::string var) const {
    return inner->derive(var);
  }
  const Expression* inner;
  ExpressionParen(const Expression* exp) : inner(exp) {}
  ExpressionParen(const ExpressionParen* root) : inner(root->getInner()){}
  ~ExpressionParen() {inner = NULL;}
};

class ExpressionOp : public Expression {
private:
  const char getOp() const {
    return this->op;
  }

  const Expression* getLeft() const {
    return this->left;
  }

  const Expression* getRight() const {
    return this->right;
  }
public:
  virtual std::string toString() const {
    std::stringstream ss;
    ss << left->toString();
    ss << op;
    ss << right->toString();
    return ss.str();
  }
  virtual double eval(const std::map<std::string, double> vars) const {
    double leftVal = left->eval(vars);
    double rightVal = right->eval(vars);
    double ret = 0.0;
    switch (this->op) {
      case '+' : {
        ret = leftVal + rightVal;
        break;
      }
      case '-' : {
        ret = leftVal - rightVal;
        break;
      }
      case '*' : {
        ret = leftVal * rightVal;
        break;
      }
      case '/' : {
        if (rightVal == 0.0) {
          throw std::invalid_argument("CANNOT DIVIDE BY 0!");
        }
        else {
          ret = leftVal / rightVal;
          break;
        }
      }
      default : {
        throw std::invalid_argument(std::to_string(op)
          + " IS NOT A VALID OPERATOR - Eval");
      }
    }
    return ret;
  }
  virtual Expression* derive(const std::string var) const {
    Expression* ret = NULL;
    const Expression* expLeft(left);
    const Expression* expRight(right);
    switch (this->op)
    {
    case '+': {
      //L' + R'
      ret = new ExpressionOp('+', expLeft->derive(var), expRight->derive(var));
      break;
    }
    case '-': {
      //L' - R'
      ret = new ExpressionOp('-', expLeft->derive(var), expRight->derive(var));
      break;
    }
    case '*': {
      //L*R' + R*L'
      ret = new ExpressionOp('+', 
                            new ExpressionOp('*', expLeft, expRight->derive(var)),
                            new ExpressionOp('*', expRight, expLeft->derive(var)));
      break;
    }
    case '/': {
      //(R*L' - L * R')/ (R*R)
      ret = new ExpressionOp('/', 
                            new ExpressionParen(
                              new ExpressionOp(
                                '-', new ExpressionOp('*', expRight, expLeft->derive(var)),
                                new ExpressionOp('*', expLeft, expRight->derive(var)))),
                            new ExpressionParen(
                            new ExpressionOp('*', expRight, expRight)));
      break;
    }
    default:
      std::stringstream ss;
      ss << this->op;
      ss << " IS NOT A VALID OPERATOR - Derive\n"
          << " Left: " << expLeft->toString() << " Right: " << expRight->toString();
      throw std::invalid_argument(ss.str());
    }
    return ret;
  }

  const Expression* clone(const Expression* root) {
    if (root == NULL) {return NULL;}
    const Expression* exp(root);
    return exp;
  }

  const char op;
  const Expression* left;
  const Expression* right;
  ExpressionOp(const char oper, const Expression* lhs, const Expression* rhs) :
    op(oper), left(lhs), right(rhs) {}

  //copy constructor
  ExpressionOp(const ExpressionOp* root) : 
    op(root->getOp()),
    left(root->getLeft()),
    right(root->getRight()) {}
  ~ExpressionOp() {left = NULL; right = NULL;}
};