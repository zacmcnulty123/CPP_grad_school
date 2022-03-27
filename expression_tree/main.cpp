#include "TreeBuilder.h"
#include <iostream>
#include <iomanip>
using namespace std;
int main(int argc, char const *argv[])
{
  std::map<std::string, double> myMap;
  myMap["x"] = 2.0;
  myMap["y"] = 3.0;
  myMap["z"] = 5.0;
  for (auto elem : myMap) {
    cout << elem.first << ": " << elem.second << "\t";
  }
  cout << endl;
  cout << "Deriving All Trees with respect to x" << endl;
  Tree* t = 
  new Add(new Mul(new Constant(2.3), new Var("x")),
    new Paren(new Mul(new Var("y"), new Paren(
      new Sub(new Var("z"), new Var("x"))))));
  cout << "Hand Built Trees" << endl;
  cout << t->toString() << endl;
  cout << std::fixed << std::setprecision(2) << t->eval(myMap) << endl;
  cout << "Derived Tree of hand built tree" << endl;
  Tree* dt = t->derive("x");
  cout << dt->toString() << endl;
  cout << std::fixed << std::setprecision(2) << dt->eval(myMap) << endl;
  cout << "Original Tree" << endl;
  cout << t->toString() << endl;
  cout << "Testing Another Hand Built Tree" << endl;
  Tree* ht =
    new Add(new Paren(new Var("x")), new Paren(new Var("y")));
  cout << ht->toString() << endl;
  cout << std::fixed << std::setprecision(2) << ht->eval(myMap) << endl;
  cout << "Derived Tree of hand built tree" << endl;
  Tree* hdt = ht->derive("x");
  cout << hdt->toString() << endl;
  cout << std::fixed << std::setprecision(2) << hdt->eval(myMap) << endl;

  cout << "TreeBuilder Tests" << endl;
  std::string tokens = "2.3 * x + ( y * ( z - x ) )";
  Tree* t2 = TreeBuilder::buildTree(tokens);
  cout << t2->toString() << endl;
  cout << std::fixed << std::setprecision(2) << t2->eval(myMap) << endl;
  Tree* dt2 = t2->derive("x");
  cout << dt2->toString() << endl;
  cout << std::fixed << std::setprecision(2) << dt2->eval(myMap) << endl;
  std::string token2 = "( 1 + z ) / x - y ";
  Tree* t3 = TreeBuilder::buildTree(token2);
  cout << t3->toString() << endl;
  Tree* dt3 = t3->derive("x");
  cout << dt3->toString() << endl;
  cout << std::fixed << std::setprecision(2) << dt3->eval(myMap) << endl;
}

