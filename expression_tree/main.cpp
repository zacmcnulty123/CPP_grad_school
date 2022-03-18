#include "TreeBuilder.h"
#include <iostream>
#include <iomanip>
using namespace std;
int main(int argc, char const *argv[])
{
  Tree* t = 
  new Add(new Mul(new Constant(2.3), new Var("x")),
    new Paren(new Mul(new Var("y"), new Paren(
      new Sub(new Var("z"), new Var("x"))))));
  cout << "Hand Built Tree" << endl;
  cout << t->toString() << endl;
  std::map<std::string, double> myMap;
  myMap["x"] = 2.0;
  myMap["y"] = 3.0;
  myMap["z"] = 5.0;
  cout << std::setprecision(2) << std::to_string(t->eval(myMap)) << endl;
  cout << "Derived Tree of hand built tree" << endl;
  Tree* dt = t->derive("x");
  cout << dt->toString() << endl;
  cout << std::setprecision(2) << std::to_string(dt->eval(myMap)) << endl;

  cout << "TreeBuilder Tests" << endl;
  std::string tokens = "2.3 * x + ( y * ( z - x ) )";
  Tree* t2 = TreeBuilder::buildTree(tokens);
  cout << t2->toString() << endl;
  cout << std::setprecision(2) << std::to_string(t2->eval(myMap)) << endl;
  Tree* dt2 = t2->derive("x");
  cout << dt2->toString() << endl;
  cout << std::setprecision(2) << std::to_string(dt2->eval(myMap)) << endl;
  std::string token2 = "( 1 + z ) / x - y ";
  Tree* t3 = TreeBuilder::buildTree(token2);
  cout << t3->toString() << endl;
  Tree* dt3 = t3->derive("x");
  cout << dt3->toString() << endl;
  cout << std::setprecision(2) << std::to_string(dt3->eval(myMap)) << endl;
}

