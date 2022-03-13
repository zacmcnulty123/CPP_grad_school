#include"tree.h"

using namespace std;
int main(void) {
  Tree t;
  t.Add(new constant(2.0), new variable("XRay"));
  return 0;
}