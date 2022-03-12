#pragma once
#include<iostream>
#include<sstream>

class Event
{
private:
  //Probability of the Event
  double prob;
public:
  //Default constructor
  Event(/* args */) : prob(0.0) {}
  //@Brief - Constructor given a value
  //@param[in] prob - Probability of the Event
  Event(double prob) {
    //Enforce Laws of Probability
    if (prob > 1.0 or prob < 0.0) {
      throw std::invalid_argument("probability must be between" 
        "0.0 and 1.0");
    }
    this->prob = prob;
  }

  //Destructor
  ~Event() {}
  
  //@Brief - Getter for the probability of the Event
  //@return double
  double getProb() const {
    return this->prob;
  }

  //@Brief - Getter for the compliment of the probability
  //@return double
  double getProbComp() const {
    return 1 - this->prob;
  }

  //@Brief - Sets the probability of the event
  //@param[in] prob - probability to set the event's probability to
  //@return void
  void setProb(double prob) {
    if (prob > 1.0 or prob < 0.0) {
      throw std::invalid_argument("probability must be between" 
        "0.0 and 1.0");
    }
    this->prob = prob;
  }

  //@Brief - toString method for the event class
  //@return std::string
  std::string toString() const {
    std::stringstream ss;
    ss << "P = " << this->prob << " ~P = " << 1 - this->getProb();
    return ss.str();
  }

};

//@Brief - Returns an event of the compliment of another Event
//Since operators all involve Events objects we want to
//return an Event over a double to utilize this operator in
//conjunction with others.
Event operator ~(const Event & A) {
  Event ret(A.getProbComp());
  return ret;
}

//@Brief - Returns the probability of two independent
// Events occuring
double operator &(const Event & A, const Event & B) {
  //P(A and B) = P(A) * P(B|A) or P(B) * P(A | B)
  //A and B are given in this case so the probabilities reduce
  //to their independent probabilities
  //P(A and B) = P(A) * P(B) or P(B) * P(A)
  return (A.getProb() * B.getProb());
}

//@Brief - Sets the probability of the lhs to the
// probability of the lhs and rhs both occuring
void operator &=(Event & A, const Event & B) {
  A.setProb((A & B));
}

//@Brief - Returns the probability of either indepdendent
//event occuring
double operator |(const Event & A, const Event & B) {
  //P(A or B) = P(A) + P(B) - P(A and B)
  return (A.getProb() + B.getProb() - (A & B));
}

//@Brief - Sets the probability of the lhs to the 
// probability of either the lhs or rhs occuring
void operator |=(Event & A, const Event & B) {
  A.setProb((A | B));
}

//@Brief - Returns the probability of where the lhs occurs and the
// rhs does not occur.
double operator ^(const Event & A, const Event & B) {
  //P(~A and ~B) = P(~A) * P(~B | ~A) ~A is given so reduces to P(~B)
  //P(~A and ~B) = P(~A) * P(~B)
  return ((~A & ~B));
}

//@Brief - Sets the probability of the lhs to the probability of
//Neither the lhs and rhs occuring
void operator ^=(Event & A, const Event & B) {
  A.setProb((A ^ B));
}

//@Brief - Returns the probability of the lhs occuring and the
// rhs not occuring
double operator -(const Event & A, const Event & B) {
  //A - B is the probability of A but not B occurring
  //P(A and ~B) = P(A - B)
  //P(A) - P(A and B)
  //P(A) - P(A)P(B)
  //P(A) * (1 - P(B))
  //P(A) * P(~B)
  return (A & ~B);
}

//@Brief - Sets the probability of the lhs to the probability of
//the lhs occuring and the rhs not occurring
void operator -=(Event & A, const Event & B) {
  A.setProb((A - B));
}

//Equal comparison operator for Events
bool operator ==(const Event & A, const Event & B) {
  return (A.getProb() == B.getProb());
}

//less than comparison operator for Events
bool operator <(const Event & A, const Event & B) {
  return (A.getProb() < B.getProb());
}

//Greater than comparison for Events
bool operator >(const Event & A, const Event & B) {
  return (A.getProb() > B.getProb());
}

//Not equal operator for Events
bool operator !=(const Event & A, const Event & B) {
  return (A.getProb() != B.getProb());
}

//Greater than Equal to comparison for Events
bool operator >=(const Event & A, const Event & B) {
  if (A == B)
  {
    return true;
  }
  else if (A > B) {
    return true;
  }
  else {
    return false;
  }
}

//Less than Equal to comparison for Events
bool operator <=(const Event & A, const Event & B) {
  if (A == B) {
    return true;
  }
  else if (A < B) {
    return true;
  }
  else {
    return false;
  }
}