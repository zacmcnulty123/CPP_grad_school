#pragma once
#include<iostream>
class Event
{
private:
  double likelihood;
public:
  Event(/* args */) : likelihood(0.0) {}
  Event(double likelihood) {
    if (likelihood > 1.0 or likelihood < 0.0) {
      throw std::invalid_argument("Likelihood must be between" 
        "0.0 and 1.0");
    }
    this->likelihood = likelihood;
  }

  ~Event() {}
  
  double getLikelihoodOfOccuring() const {
    return this->likelihood;
  }

  double getLikelihoodOfNotOccuring() const {
    return 1.0 - this->likelihood;
  }

  void setLikelihood(double likelihood) {
    if (likelihood > 1.0 or likelihood < 0.0) {
      throw std::invalid_argument("Likelihood must be between" 
        "0.0 and 1.0");
    }
    this->likelihood = likelihood;
  }

  double getAandBLikelihood(const Event & B) const {
    //P(A and B) = P(A) * P(B|A) or P(B) * P(A | B)
    //A and B are given in this case so the probabilities reduce
    //to their independent probabilities
    //P(A and B) = P(A) * P(B) or P(B) * P(A)
    return (this->likelihood * B.getLikelihoodOfOccuring());
  }

  double getAorBLikelihood(const Event & B) const {
    //P(A or B) = P(A) + P(B) - P(A and B)
    return (this->likelihood + B.getLikelihoodOfOccuring() - getAandBLikelihood(B));
  }

  double getAnorBLikelihood(const Event & B) const {
    //P(~A and ~B) = P(~A) * P(~B | ~A) ~A is given so reduces to P(~B)
    //P(~A and ~B) = P(~A) * P(~B)
    return (this->getLikelihoodOfNotOccuring() * B.getLikelihoodOfNotOccuring());
  }

  double getANotBLikelihood(const Event & B) const {
    //A - B is the probability of A but not B occurring
    //P(A and ~B) = P(A - B)
    //P(A) - P(A and B)
    //P(A) - P(A)P(B)
    //P(A) * (1 - P(B))
    //P(A) * P(~B)
    return (this->likelihood * B.getLikelihoodOfNotOccuring());
  }

  double operator &(const Event & B) {
    return (this->getAandBLikelihood(B));
  }

  double operator |(const Event & B) {
    return (this->getAorBLikelihood(B));
  }

  double operator ^(const Event & B) {
    return (this->getAnorBLikelihood(B));
  }

  double operator -(const Event & B) {
    return (this->getANotBLikelihood(B));
  }
};