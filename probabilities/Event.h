#pragma once
#include<iostream>
#include<sstream>
class Event
{
private:
  double prob;
public:
  Event(/* args */) : prob(0.0) {}
  Event(double prob) {
    if (prob > 1.0 or prob < 0.0) {
      throw std::invalid_argument("probability must be between" 
        "0.0 and 1.0");
    }
    this->prob = prob;
  }

  ~Event() {}
  
  double getProb() const {
    return this->prob;
  }

  double getNotProb() const {
    return 1.0 - this->prob;
  }

  void setProb(double prob) {
    if (prob > 1.0 or prob < 0.0) {
      throw std::invalid_argument("probability must be between" 
        "0.0 and 1.0");
    }
    this->prob = prob;
  }

  std::string toString() const {
    std::stringstream ss;
    ss << "P = " << this->prob << " ~P = " << this->getNotProb();
    return ss.str();
  }

};

Event operator ~(const Event & A) {
  Event ret(A.getNotProb());
  return ret;
}

double operator &(const Event &A, const Event & B) {
  //P(A and B) = P(A) * P(B|A) or P(B) * P(A | B)
  //A and B are given in this case so the probabilities reduce
  //to their independent probabilities
  //P(A and B) = P(A) * P(B) or P(B) * P(A)
  return (A.getProb() * B.getProb());
}

double operator &=(const Event & A, const Event & B) {
  return (A & B);
}

double operator |(const Event & A, const Event & B) {
  //P(A or B) = P(A) + P(B) - P(A and B)
  return (A.getProb() + B.getProb() - (A & B));
}

double operator |=(const Event & A, const Event & B) {
  return (A | B);
}

double operator ^(const Event & A, const Event & B) {
  //P(~A and ~B) = P(~A) * P(~B | ~A) ~A is given so reduces to P(~B)
  //P(~A and ~B) = P(~A) * P(~B)
  return (A.getNotProb() + B.getNotProb() - (~A & ~B));
}

double operator ^=(const Event & A, const Event & B) {
  return (A ^ B);
}

double operator -(const Event & A, const Event & B) {
  //A - B is the probability of A but not B occurring
  //P(A and ~B) = P(A - B)
  //P(A) - P(A and B)
  //P(A) - P(A)P(B)
  //P(A) * (1 - P(B))
  //P(A) * P(~B)
  return (A & ~B);
}

double operator -=(const Event & A, const Event & B) {
  return (A - B);
}

bool operator ==(const Event & A, const Event & B) {
  return (A.getProb() == B.getProb());
}

bool operator <(const Event & A, const Event & B) {
  return (A.getProb() < B.getProb());
}

bool operator >(const Event & A, const Event & B) {
  return (A.getProb() > B.getProb());
}

bool operator !=(const Event & A, const Event & B) {
  return (A.getProb() != B.getProb());
}

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