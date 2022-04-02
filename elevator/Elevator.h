#pragma once
#include "passenger.h"
#include <vector>
enum class elevatorOp {STOPPED, STOPPING, MOVING_UP, MOVING_DOWN};
class Elevator
{
private:
  /* data */
  unsigned int elevNum;
  unsigned int currFloor;
  std::vector<Passenger> passengers;
  elevatorOp elevOp;
public:
  Elevator(
    const unsigned int & en,
    const unsigned int & cf = 0) :
      elevNum(en),
      currFloor(cf),
      passengers(std::vector<Passenger>()),
      elevOp(elevatorOp::STOPPED) {}
  ~Elevator() {}

  unsigned int getCurrFloor() const {
    return this->currFloor;
  }

  unsigned int getNumPassengers() const {
    return static_cast<unsigned int>(this->passengers.size());
  }

  elevatorOp getElevatorState() const {
    return this->elevOp;
  }

  void setElevatorState(const elevatorOp & op) {
    this->elevOp = op;
  }

  unsigned int getElevNum() const  {
    return this->elevNum;
  }
};
