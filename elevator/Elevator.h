#pragma once
#include "passenger.h"
#include <vector>
enum class elevatorOp {STOPPED, STOPPING, MOVING_UP, MOVING_DOWN};
class Elevator
{
private:
  /* data */
  int elevNum;
  int currFloor;
  std::vector<Passenger> passengers;
  elevatorOp elevOp;
public:
  Elevator(
    const int & en,
    const int & cf = 0) :
      elevNum(en),
      currFloor(cf),
      passengers(std::vector<Passenger>()),
      elevOp(elevatorOp::STOPPED) {}
  ~Elevator() {}

  int getCurrFloor() const {
    return this->currFloor;
  }

  int getNumPassengers() const {
    return static_cast<int>(this->passengers.size());
  }

  elevatorOp getElevatorState() const {
    return this->elevOp;
  }

  void setElevatorState(const elevatorOp & op) {
    this->elevOp = op;
  }

  int getElevNum() const  {
    return this->elevNum;
  }
};
