#pragma once
#include <iostream>
#include <string>

class Passenger
{
private:
  //Start time for the passenger
  int startTime;
  //Floor the passenger starts on
  int startFloor;
  //Floor the passenger ends on
  int endFloor;
  //Time the passenger got on the elevator
  int startTravelTime;
public:
  //constructor
  Passenger() {
    startTime = 0;
    startFloor = 0;
    endFloor = 0;
  }
  //@Brief - constructor
  //@Param[in] st - when the passenger arrives at
  // the elevator door for the given floor
  //@Param[in] sf - Floor the passenger is starting at
  //@Param[in] ef - Floor the passenger is ending at
  Passenger(
    const int & st,
    const int & sf,
    const int & ef) :
      startTime(st),
      startFloor(sf),
      endFloor(ef) {}
  //Destructor
  ~Passenger() {}

  //@Brief - Gets the passengers start time
  int getStartTime() const {
    return this->startTime;
  }

  //@Brief - Gets the passenger's starting floor
  int getStartFloor() const {
    return this->startFloor;
  }

  //@Brief - Gets the passenger's end floor
  int getEndFloor() const {
    return this->endFloor;
  }

  //@Brief - set start time
  void setStartTime(const int & st) {
    this->startTime = st;
  }

  //@Brief - sets end time
  void setStartFloor(const int & sf) {
    this->startFloor = sf;
  }

  //@Brief - sets end floor
  void setEndFloor(const int & ef) {
    this->endFloor = ef;
  }

  //@Brief - Sets the passenger's start travel time
  //@Param[in] currTime - time passenger getting on the elevator
  void setStartTravelTime(const int & currTime) {
    this->startTravelTime = currTime;
  }

  //@Brief - Gets the start travel time
  int getStartTravelTime() const {
    return this->startTravelTime;
  }

  //@Brief - toString method for passenger class
  //@return std::string
  std::string toString() const {
    std::stringstream ss;
    ss << "Start Time: " << this->getStartTime()
    << " Start Floor: " << this->getStartFloor()
    << " End Floor: " << this->getEndFloor()
    << std::endl;
    return ss.str();
  }
};

//Operators to help with std::sorting methods
bool operator >(const Passenger &a, const Passenger &b) {
  return (a.getStartTime() > b.getStartTime());
}

bool operator <(const Passenger &a, const Passenger &b) {
  return (a.getStartTime() < b.getStartTime());
}

bool operator ==(const Passenger &a, const Passenger &b) {
  return (a.getStartTime() == b.getStartTime());
}

bool operator <=(const Passenger &a, const Passenger &b) {
  return (a.getStartTime() <= b.getStartTime());
}

bool operator >=(const Passenger &a, const Passenger &b) {
  return (a.getStartTime() >= b.getStartTime());
}

//StringStream operator overload for loading in csv file contents into the
//Passenger class object
std::stringstream & operator >>(std::stringstream & ss, Passenger & out) {
  std::string st, sf, ef;
  std::getline(ss, st, ',');
  std::getline(ss, sf, ',');
  std::getline(ss, ef, ',');
  //stoi was throwing an invalid argument exception despite
  //successfully converting the string.
  try { 
    out.setStartTime(std::stoi(st));
    out.setStartFloor(std::stoi(sf));
    out.setEndFloor(std::stoi(ef));
  }
  catch (std::invalid_argument & e) {}
  return ss;
}

std::ostream & operator << (std::ostream &out, const Passenger & ps) {
  out << ps.toString();
  return out;
}