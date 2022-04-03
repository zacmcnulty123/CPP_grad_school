#pragma once
#include <queue>
#include <iostream>
#include "passenger.h"

enum class FloorState {SELECTED, NOT_SELECTED};
class Floor
{
  private:
    /* data */
    int floorNumber;
    std::deque<Passenger> elevatorQue;
    FloorState state;
  public:
    Floor(const int & fn) :
      floorNumber(fn),
      elevatorQue(std::deque<Passenger>()),
      state(FloorState::NOT_SELECTED){}
    ~Floor() {}

    void queuePassenger(const Passenger & ps) {
      elevatorQue.push_back(ps);
      this->sortQueue();
    }

    void dequePassengers(const int & numToRemove) {
      for (int i = 0; i < numToRemove; ++i) {
        elevatorQue.pop_front();
      }
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

    Passenger getFirstPassenger() const {
      if (elevatorQue.size() == 0) {
        throw std::invalid_argument("Floor's Queue is empty");
      }
      else {
        return elevatorQue.at(0);
      }
    }

    int getFloorNumber() const {
      return this->floorNumber;
    }

    FloorState getFloorState() const {
      return this->state;
    }

    int getNumPassenger() const {
      return this->elevatorQue.size();
    }

    std::deque<Passenger> getPassengers() const {
      return this->elevatorQue;
    }

    void setState(const FloorState & fs) {
      this->state = fs;
    }
};

std::ostream & operator <<(std::ostream &out, const Floor & floor) {
  out << floor.toString();
  return out;
}

//Want to be able to sort floors based on passenger priority
bool operator >(const Floor & a, const Floor &b) {
  if (a.getFloorState() == FloorState::SELECTED) {
    return false;
  }
  else if (b.getFloorState() == FloorState::SELECTED) {
    return true;
  }
  else if (a.getNumPassenger() > 0 and b.getNumPassenger() == 0) {
    return true;
  }
  else if (b.getNumPassenger() > 0 and a.getNumPassenger() == 0) {
    return false;
  }
  else if (a.getNumPassenger() == 0 and b.getNumPassenger() == 0) {
    return false;
  }
  else {
    return (a.getFirstPassenger().getStartTime() > 
      b.getFirstPassenger().getStartTime());
    }
}


bool operator <(const Floor & a, const Floor & b) {
  if (a.getFloorState() == FloorState::SELECTED) {
    return false;
  }
  else if (b.getFloorState() == FloorState::SELECTED) {
    return true;
  }
  else if (a.getNumPassenger() > 0 and b.getNumPassenger() == 0) {
    return true;
  }
  else if (b.getNumPassenger() > 0 and a.getNumPassenger() == 0) {
    return false;
  }
  else if (a.getNumPassenger() == 0 and b.getNumPassenger() == 0) {
    return false;
  }
  else {
    return (a.getFirstPassenger().getStartTime() <
      b.getFirstPassenger().getStartTime());
  }
}