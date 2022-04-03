#pragma once
#include <iostream>
#include <string>

enum class passState {WAITING, NOT_QUEUED, IN_TRANSIT, DONE};

class Passenger
{
  private:
    int startTime;
    int endTime;
    int startFloor;
    int endFloor;
    int waitTime;
    int travelTime;
    int startTravelTime;
    passState status;
  public:
    Passenger() {
      startTime = 0;
      startFloor = 0;
      endFloor = 0;
      status = passState::NOT_QUEUED;
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
        endFloor(ef),
        status(passState::NOT_QUEUED) {}
    //Destructor
    ~Passenger() {}

    int getStartTime() const {
      return this->startTime;
    }

    int getStartFloor() const {
      return this->startFloor;
    }

    int getEndFloor() const {
      return this->endFloor;
    }

    int getEndTime() const {
      return this->endTime;
    }

    void setEndTime(const int & et) {
      this->endTime = et;
    }

    void setStartTime(const int & st) {
      this->startTime = st;
    }

    void setStartFloor(const int & sf) {
      this->startFloor = sf;
    }

    void setEndFloor(const int & ef) {
      this->endFloor = ef;
    }

    void setWaitTime(const int & currTime) {
      this->waitTime = currTime - this->startTime;
      this->startTravelTime = currTime;
    }

    int getWaitTime() const {
      return this->waitTime;
    }

    passState getPassState() const {
      return this->status;
    }

    void setTravelTime(const int & currTime) {
      this->travelTime = currTime - this->startTravelTime;
    }

    int getTravelTime() const {
      return this->travelTime;
    }

    std::string toString() const {
      std::stringstream ss;
      ss << "Start Time: " << this->getStartTime()
      << " Start Floor: " << this->getStartFloor()
      << " End Floor: " << this->getEndFloor()
      << std::endl;
      return ss.str();
    }
};

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