#pragma once
#include "passenger.h"
#include "Floor.h"
#include <vector>
//Elevator can be in several states
// Stopped without passengers so it needs to for a new destination and
// begin moving to that destination
// the elevator can be stopped while going down and stopped while going up
// The elevator can be stopping while moving up and stopping while moving down
// The elevator can be moving up or moving down
enum class ElevatorOp {STOPPED_NO_PASS, STOPPED_UP, STOPPED_DOWN,
  STOPPING_UP, STOPPING_DOWN, MOVING_UP, MOVING_DOWN};

std::ostream & operator <<(std::ostream & out, const ElevatorOp & op) {
  switch (op)
  {
  case ElevatorOp::STOPPED_NO_PASS: {
    out << "Stopped with no passengers" << std::endl;
    break;
  }
  case ElevatorOp::STOPPED_UP: {
    out << "stopped going up" << std::endl;
    break;
  }
  case ElevatorOp::STOPPED_DOWN: {
    out << "stopped going down" << std::endl;
    break;
  }
  case ElevatorOp::STOPPING_UP: {
    out << "stopping going up" << std::endl;
    break;
  }
  case ElevatorOp::STOPPING_DOWN: {
    out << "stopping going down" << std::endl;
    break;
  }
  case ElevatorOp::MOVING_UP: {
    out << "moving up" << std::endl;
    break;
  }
  case ElevatorOp::MOVING_DOWN: {
    out << "moving down" << std::endl;
  }
  default:
    break;
  }
  return out;
}

class Elevator
{
private:
  //ID for elevator
  int elevNum;
  //Current floor the elevator is on
  int currFloor;
  //Floor the elevator is going
  int nextDestination;
  //Time it takes the elevator to go between floors
  int timeBtwFloors;
  //time in the current state
  int timeInState;
  //Passengers on elevator (chosen because of the erase method)
  std::vector<Passenger> passengers;
  //Current Elevator operation
  ElevatorOp elevOp;

  //Time it takes to stop
  const int TIME_TO_STOP = 2;

  //@Brief - Increments the times in state by 1
  void incrementTimeInState() {
    this->timeInState++;
  }

  //@Brief - Resets time in state to 0
  void resetTimeInState() {
    this->timeInState = 0;
  }

  //@Brief - Gets the time spent in current state
  int getTimeInState() const {
    return this->timeInState;
  }
public:
  //Dictates the maximum capacity of the elevator
  const static int MAX_CAPACITY = 8;

  //Constructor (Floors are 1 based)
  Elevator(
    const int & en,
    const int & tbf = 10,
    const int & cf = 1) :
      elevNum(en),
      timeBtwFloors(tbf),
      timeInState(0),
      currFloor(cf),
      nextDestination(0),
      passengers(std::vector<Passenger>()),
      elevOp(ElevatorOp::STOPPED_NO_PASS) {}
  ~Elevator() {}

  //@Brief - Returns the floor the elevator is on
  int getCurrFloor() const {
    return this->currFloor;
  }

  //@Brief - Returns the number of passengers in the elevator
  int getCurrNumPassengers() const {
    return this->passengers.size();
  }

  //@Brief - Returns the operational state of the Elevator
  ElevatorOp getElevatorState() const {
    return this->elevOp;
  }

  //@Brief - Sets the elevator's state
  void setElevatorState(const ElevatorOp & op) {
    timeInState = 0;
    this->elevOp = op;
  }

  //@Brief - Returns the elevator's ID
  int getElevNum() const {
    return this->elevNum;
  }

  //@Brief - Added a passenger to the elevator if
  // the elevator is not at capacity
  //@Param[in] ps - Passenger to add
  //@return bool - it was possible to add passenger
  bool addPassenger(const Passenger & ps) {
    if (this->getCurrNumPassengers() >= MAX_CAPACITY) {
      return false;
    }
    else {
      passengers.push_back(ps);
      return true;
    }
  }

  //@Brief - Returns a list of passengers that were
  // removed from the elevator
  std::deque<Passenger> removePassengers() {
    std::deque<Passenger> ret;
    std::vector<Passenger>::iterator i = this->passengers.begin();
    for (Passenger & pass : this->passengers) {
      //If we're at the floor the passengers needs to get off at
      // they can get off.
      if (this->currFloor == pass.getEndFloor()) {
        ret.push_back(pass);
        this->passengers.erase(i);
      }
      else {
        i++;
      }
    }
    return ret;
  }

  //@Brief - Sets the elevator's next destination
  //@Param[in] - fn - Floor number to go to
  void setNextDestination(const int & fn) {
    this->nextDestination = fn;
    //Set state to move up or down
    if (this->currFloor - this->nextDestination < 0) {
      this->setElevatorState(ElevatorOp::MOVING_UP);
    }
    else {
      this->setElevatorState(ElevatorOp::MOVING_DOWN);
    }
  }

  //@Brief - Determines the next floor to stop at
  // Assumes that there is a passenger on the elevator
  void determineNextDestination() {
    if (this->getCurrNumPassengers() == 0) {
      this->setElevatorState(ElevatorOp::STOPPED_NO_PASS);
      return;
    }
    int nextDest = 0;
    switch (this->getElevatorState()) {
      //Do nothing
      case ElevatorOp::STOPPED_NO_PASS: {break;}
      case ElevatorOp::STOPPED_UP: {
        //Find the closest floor above us
        // if none are up redo to the stopped moving
        // down state and redetermine
        int closest = std::numeric_limits<int>::max();
        for (const Passenger & pass : this->passengers) {
          if (pass.getEndFloor() - currFloor < closest and
              pass.getEndFloor() - currFloor > 0) {
            closest = pass.getEndFloor() - currFloor;
            nextDest = pass.getEndFloor();
          }
        }
        if (nextDest == 0) {
          this->setElevatorState(ElevatorOp::STOPPED_DOWN);
          this->determineNextDestination();
        }
        else {
          this->setNextDestination(nextDest);
        }
        break;
      }
      case ElevatorOp::STOPPED_DOWN: {
        //Same and STOPPED_UP but if no floor is
        // below us switch to STOPPED_UP and redo
        int closest = std::numeric_limits<int>::min();
        for (const Passenger & pass : this->passengers) {
          if (pass.getEndFloor() - currFloor > closest and
            pass.getEndFloor() - currFloor < 0) {
            closest = pass.getEndFloor() - currFloor;
            nextDest = pass.getEndFloor();
          }
        }
        if (nextDest == 0) {
          this->setElevatorState(ElevatorOp::STOPPED_UP);
          this->determineNextDestination();
          break;
        }
        else {
          this->setNextDestination(nextDest);
        }
        break;
      }
      //Do nothing
      case ElevatorOp::STOPPING_UP: {}
      case ElevatorOp::STOPPING_DOWN: {}
      case ElevatorOp::MOVING_UP: {}
      case ElevatorOp::MOVING_DOWN: {break;}
      default:
      {
        throw std::invalid_argument("Invalid Elevator Operation State!");
        break;
      }
    }
  }

  //@Brief - Check to see if the elevator should transition states
  bool checkStateTransition() {
    bool transition = false;
    switch (this->getElevatorState()) {
      case ElevatorOp::STOPPED_NO_PASS: {
        // Do nothing handled with another method
        break;
      }
      case ElevatorOp::STOPPED_UP: {
        if (this->getCurrNumPassengers() == 0) {
          this->setElevatorState(ElevatorOp::STOPPED_NO_PASS);
        }
        transition = true;
        break;
      }
      case ElevatorOp::STOPPED_DOWN: {
        if (this->getCurrNumPassengers() == 0) {
          this->setElevatorState(ElevatorOp::STOPPED_NO_PASS);
        }
        transition = true;
        break;
      }
      case ElevatorOp::STOPPING_UP: {
        if (this->getTimeInState() % TIME_TO_STOP == 0) {
          this->setElevatorState(ElevatorOp::STOPPED_UP);
          this->resetTimeInState();
          this->currFloor++;
          transition = true;
        }
        else {
          break;
        }
        break;
      }
      case ElevatorOp::STOPPING_DOWN: {
        if (this->getTimeInState() % TIME_TO_STOP == 0) {
          this->setElevatorState(ElevatorOp::STOPPED_DOWN);
          this->resetTimeInState();
          this->currFloor--;
          transition = true;
        }
        else {
          break;
        }
        break;
      }
      case ElevatorOp::MOVING_UP: {
        if (this->nextDestination - this->currFloor == 1) {
          if (this->getTimeInState() % timeBtwFloors == 0) {
            this->setElevatorState(ElevatorOp::STOPPING_UP);
            this->resetTimeInState();
            transition = true;
          }
        }
        else {
          if (this->getTimeInState() % timeBtwFloors == 0) {
            this->resetTimeInState();
            this->currFloor++;
          }
        }
        break;
      }
      case ElevatorOp::MOVING_DOWN: {
        if (this->currFloor - this->nextDestination == 1) {
          if (this->getTimeInState() % timeBtwFloors == 0) {
            this->setElevatorState(ElevatorOp::STOPPING_DOWN);
            this->resetTimeInState();
            transition = true;
          }
        }
        else {
          if (this->getTimeInState() % timeBtwFloors == 0) {
            this->resetTimeInState();
            this->currFloor--;
          }
        }
        break;
      }
      default:
      {
        throw std::invalid_argument("Invalid Elevator Operation State!");
        break;
      }
    }
    this->incrementTimeInState();
    return transition;
  }

  //@Brief - to string method for elevator
  //@return std::string
  std::string toString() const {
    std::stringstream ss;
    ss << "Elevator: " << this->getElevNum() << " "
    << this->getElevatorState() <<
    " next destination is floor: " << this->nextDestination
    << " currently on floor: "
    << this->currFloor << " have been in current state for: "
    << this->getTimeInState()
    << " time between floors: " << timeBtwFloors;
    for (const Passenger & pass : this->passengers) {
      ss << "\n" << pass;
    }
    return ss.str();
  }
};

std::ostream & operator <<(std::ostream & out, const Elevator & ev) {
  out << ev.toString() << std::endl;
  return out;
}