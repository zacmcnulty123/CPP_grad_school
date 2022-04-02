#pragma once
#include <iostream>
class Passenger
{
  private:
    unsigned int startTime;
    unsigned int endTime;
    unsigned int startFloor;
    unsigned int endFloor;
  public:
    Passenger() {}
    //@Brief - constructor
    //@Param[in] st - when the passenger arrives at
    // the elevator door for the given floor
    //@Param[in] sf - Floor the passenger is starting at
    //@Param[in] ef - Floor the passenger is ending at
    Passenger(
      const unsigned int & st,
      const unsigned int & sf,
      const unsigned int & ef) :
        startTime(st),
        startFloor(sf),
        endFloor(ef) {}
    //Destructor
    ~Passenger() {}

    unsigned int getStartTime() const {
      return this->startTime;
    }

    unsigned int getStartFloor() const {
      return this->startFloor;
    }

    unsigned int getEndFloor() const {
      return this->endFloor;
    }

    unsigned int getEndTime() const {
      return this->endTime;
    }

    void setEndTime(const unsigned int & et) {
      this->endTime = et;
    }

    void setStartTime(const unsigned int & st) {
      this->startTime = st;
    }

    void setStartFloor(const unsigned int & sf) {
      this->startFloor = sf;
    }

    void setEndFloor(const unsigned int & ef) {
      this->endFloor = ef;
    }

    std::string toString() const {
      std::stringstream ss;
      ss << "Start Time: " << this->getStartTime()
      << " Start Floor: " << this->getStartFloor()
      << " End Floor: " << this->getEndFloor();
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

std::stringstream & operator >>(std::stringstream & in, Passenger & out) {
  unsigned int st;
  unsigned int sf;
  unsigned int ef;
  in >> st >> sf >> ef;
  out.setStartTime(st);
  out.setStartFloor(sf);
  out.setEndFloor(ef);
  return in;
}

std::ostream & operator << (std::ostream &out, const Passenger & ps) {
  out << ps.toString();
  return out;
}