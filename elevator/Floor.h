#pragma once
#include <queue>
#include <iostream>
#include "passenger.h"
class Floor
{
  private:
    /* data */
    int floorNumber;
    std::deque<Passenger> elevatorQue;
  public:
    Floor(const int & fn) :
      floorNumber(fn) {}
    ~Floor() {}

    void queuePassenger(const Passenger & ps) {
      elevatorQue.push_back(ps);
    }

    void dequePassenger() {
      elevatorQue.pop_front();
    }

    void sortQueue() {
      std::sort(elevatorQue.begin(), elevatorQue.end());
    }

    std::string toString() const {
      std::stringstream ss;
      ss << "Floor #: " << this->floorNumber << " queues passenger infos \n";
      for (Passenger ps : elevatorQue) {
        ss << ps << std::endl;
      }
      return ss.str();
    }
};

std::ostream & operator <<(std::ostream &out, const Floor & floor) {
  out << floor.toString();
  return out;
}