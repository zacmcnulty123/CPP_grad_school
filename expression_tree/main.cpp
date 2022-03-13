#include"expression.h"
#include<iostream>
using namespace std;
int main(int argc, char const *argv[])
{
  Expression* t = new ExpressionOp('+', new ExpressionConst(2.0), new ExpressionVar("a"));
  cout << t->toString() << endl;
  std::map<std::string, double> myMap;
  myMap["a"] = 3.0;
  cout << t->eval(myMap);
  return 0;
}
