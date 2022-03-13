#include"expression.h"
#include<iostream>
using namespace std;
int main(int argc, char const *argv[])
{
  Expression* t = 
  new ExpressionOp('+', new ExpressionOp('*', new ExpressionConst(2.3), new ExpressionVar("x")),
    new ExpressionParen(new ExpressionOp('*', new ExpressionVar("y"), 
    new ExpressionParen(new ExpressionOp('-', new ExpressionVar("z"), new ExpressionVar("x"))))));
  cout << t->toString() << endl;
  std::map<std::string, double> myMap;
  myMap["x"] = 2.0;
  myMap["y"] = 3.0;
  myMap["z"] = 5.0;
  cout << std::to_string(t->eval(myMap)) << endl;

  Expression* dt = t->derive("x");
  cout << dt->toString() << endl;
  cout << std::to_string(dt->eval(myMap)) << endl;
  return 0;
}
