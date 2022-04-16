#include "Deck.h"

using namespace std;
int main(int argc, char const *argv[])
{
  Deck deck = Deck();

  cout << "Original Deck" << deck;

  deck.shuffle();

  cout << "Shuffled Deck" << deck;
  return 0;
}
