#include "Tree.h"
#include <iostream>
#include <iomanip>
using namespace std;
int main(int argc, char const *argv[])
{
  Tree* t = 
  new Add(new Mul(new Constant(2.3), new Var("x")),
    new Paren(new Mul(new Var("y"), new Paren(
      new Sub(new Var("z"), new Var("x"))))));
  cout << t->toString() << endl;
  std::map<std::string, double> myMap;
  myMap["x"] = 2.0;
  myMap["y"] = 3.0;
  myMap["z"] = 5.0;
  cout << std::setprecision(2) << std::to_string(t->eval(myMap)) << endl;

  Tree* dt = t->derive("x");
  cout << dt->toString() << endl;
  cout << std::setprecision(2) << std::to_string(dt->eval(myMap)) << endl;
  cout << t->toString() << endl;
  cout << std::setprecision(2) << std::to_string(t->eval(myMap)) << endl;
  return 0;
}
