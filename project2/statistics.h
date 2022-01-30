#pragma once
#include<vector>
#include<ostream>
#include<string>
class statistics {
private:
  std::vector<double> seq;
public:
  //Constructor
  statistics();
  //Destructor
  ~statistics();


  // @Brief - pushes back a double to the seq queue
  // @Param[in] x - double to push onto the queue
  // @return void
  void enqueue(double x);
  
  //@Brief - Returns the sum of all elements of the sequence
  //@Return - double the sum of all elements of the sequence
  double get_sum() const;

  // @Brief returns the standard deviation of the sequence
  // @return double - Standard deviation of the sequence
  double get_STD() const;

  //@Brief - returns the mean of the sequence
  //@return double - the mean fo the sequence
  double get_mean() const;

  //@Brief - returns the median of the sequence
  //@return double - The median of the sequence
  double get_median() const;

  //@Brief - returns the mode of the sequence
  //@return double - The mode of the sequence
  double get_mode() const;

  //@Brief - returns the quartiles of the sequence
  //@return - returns the quartiles of the sequence
  double get_quartiles() const;

  //@Brief - returns the variation of the sequence
  //@return double - the variation of the sequence
  double get_variance() const;

  //@Brief - Returns a string of specified range
  //@Param[in] start - desired start index of the sequences
  //            default 0
  //@param[in] stop - desired stop point of the sequence
  //            default -1: Specifies to do entire sequence length
  std::string to_string(const int start = 0, const int stop = -1) const;

  //@Brief - Ensures that the statistics class has data
  // that can be worked on
  //@Param[in] string - calling function name
  bool is_ready(const std::string func_name) const;
};

std::ostream & operator<< (std::ostream &out, const statistics & seq) {
  out << seq.to_string();
  return out;
}