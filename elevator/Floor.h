#pragma once
#include <queue>
#include <iostream>
#include "passenger.h"

//Helps with priority of an elevator prioritizing a floor
enum class FloorState {SELECTED, NOT_SELECTED};
class Floor
{
private:
  //Id for the floor
  int floorNumber;
  //Queue of passengers on the floor
  std::deque<Passenger> elevatorQue;
  //If the floor is a priority of an elevator or not
  FloorState state;

  //@Brief - sort the floor's queue
  void sortQueue() {
    std::sort(elevatorQue.begin(), elevatorQue.end());
  }
public:

  //Constructor
  Floor(const int & fn) :
    floorNumber(fn),
    elevatorQue(std::deque<Passenger>()),
    state(FloorState::NOT_SELECTED){}
  //Destructor
  ~Floor() {}
  
  //@Brief - Put a passenger in line for the elevator
  //@Param[in] ps - passenger to queue
  void queuePassenger(const Passenger & ps) {
    elevatorQue.push_back(ps);
  }

  //@Brief - remove the passenger from the queue
  //@Param[in] numToRemove - number of passengers to remove
  void dequePassengers(const int & numToRemove) {
    for (int i = 0; i < numToRemove; ++i) {
      elevatorQue.pop_front();
    }
  }

  //@Brief - toString method for floor
  //@return std::string
  std::string toString() const {
    std::stringstream ss;
    ss << "Floor #: " << this->floorNumber << " queues passenger infos \n";
    for (Passenger ps : elevatorQue) {
      ss << ps << std::endl;
    }
    return ss.str();
  }

  //@Brief - return the first passenger of the queue
  //@return Passenger
  Passenger getFirstPassenger() const {
    if (elevatorQue.size() == 0) {
      throw std::invalid_argument("Floor's Queue is empty");
    }
    else {
      return elevatorQue.at(0);
    }
  }

  //@Brief - Gets the floor number of the floor
  int getFloorNumber() const {
    return this->floorNumber;
  }

  //@Brief - Get the state of the floor
  FloorState getFloorState() const {
    return this->state;
  }

  //@Brief - gets the number fo passengers on the floor
  int getNumPassenger() const {
    return this->elevatorQue.size();
  }

  //@Brief - Returns the list of passengers on the floor
  std::deque<Passenger> getPassengers() const {
    return this->elevatorQue;
  }

  //@Brief - Sets the states of the floor
  //@Param[in] fs - State to set the floor to
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

//Want to be able to sort floor to choose floors by priority
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