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
  Expression(){}
  ~Expression(){}
};

class ExpressionConst : public Expression {
  public:
    virtual std::string toString() const {
      std::stringstream ss;
      ss << value;
      return ss.str();
    }
    virtual double eval(const std::map<std::string, double> /*vars*/) const {
      return this->value;
    }
    const double value;
    ExpressionConst(double val) : value(val) {}
    ~ExpressionConst(){}
};

class ExpressionVar : public Expression {
  public:
    virtual std::string toString() const {
      return var;
    }
    virtual double eval(const std::map<std::string, double> vars) const {
      return vars.at(this->var);
    }
    const std::string var;
    ExpressionVar(std::string val) : var(val) {}
    ~ExpressionVar(){}
};

class ExpressionMinus : public Expression {
  public:
    virtual std::string toString() const {
      std::stringstream ss;
      ss << "-";
      ss << inner->toString();
      return ss.str();
    }
    virtual double eval(const std::map<std::string, double> vars) const {
      return -1 * (inner->eval(vars));
    }
    const Expression* inner;
    ExpressionMinus(Expression* exp) : inner(exp) {}
    ~ExpressionMinus() {inner = NULL;}
};

class ExpressionParen : public Expression {
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
    const Expression* inner;
    ExpressionParen(Expression* exp) : inner(exp) {}
    ~ExpressionParen() {inner = NULL;}
};

class ExpressionOp : public Expression {
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
      switch (op) {
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
            + " IS NOT A VALID OPERATOR");
        }
      }

      return ret;
    }
    const char op;
    const Expression* left;
    const Expression* right;
    ExpressionOp(char oper, Expression* lhs, Expression* rhs) :
      op(oper), left(lhs), right(rhs) {}
    ~ExpressionOp() {left = NULL; right = NULL;}
};