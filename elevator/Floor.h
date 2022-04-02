#pragma once
#include <queue>
#include "passenger.h"
class Floor
{
  private:
    /* data */
    unsigned int floorNumber;
    std::queue<Passenger> elevatorQue;
  public:
    Floor(const unsigned int & fn) :
      floorNumber(fn) {}
    ~Floor() {}

    void queuePassenger(const Passenger & ps) {
      elevatorQue.push(ps);
    }

    void dequePassenger() {
      elevatorQue.pop();
    }
};