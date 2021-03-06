#pragma once
#include<vector>
#include<ostream>
#include<string>
#include<map>
#include<type_traits>
template <class T>
class statistics {
private:
  //Enforce Arithmetic only template type
  //Causes compile errors if template is instantiated
  //with non-arithmetic type
  typename std::enable_if<std::is_arithmetic<T>::value, T>::type inner;
  std::vector<T> seq;
  std::map<T, unsigned int> mode;

  //@Brief - returns the median of the sequence
  //@param[in] vec - Vector to get the median for
  //@return double - The median of the sequence
  T get_median(std::vector<T> vec) const;
public:
  //Constructor
  statistics();

  //Constructor that allows the class to take in an already created
  //vector of numbers
  statistics(std::vector<T> vec);
  //Destructor
  ~statistics();

  // @Brief - pushes back a number to the seq queue
  // @Param[in] x - double to push onto the queue
  // @return void
  void enqueue(T x);
  
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
  T get_median() const;

  //@Brief - returns the mode of the sequence
  //@return double - The mode of the sequence
  T get_mode() const;

  //@Brief - returns the quartiles of the sequence
  //@return - returns the quartiles of the sequence
  std::map<std::string, T> get_quartiles() const;

  //@Brief - returns the variation of the sequence
  //@return double - the variation of the sequence
  double get_variance() const;

  //@Brief - returns a string formatted to have the
  //Descriptive statistics for the sequence
  //@return - std::string. Formatted string for the
  //Descriptive statistics
  std::string get_descriptive_statistics() const;

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

template<typename T>
std::ostream & operator<< (std::ostream &out, const statistics<T> & seq) {
  out << seq.to_string();
  return out;
}

template<typename T>
std::ostream &operator<<(std::ostream &out, const std::map<std::string, T> &quart) {
  for (const auto& elem : quart) {
    out << elem.first << ": " << elem.second << std::endl;
  }
  return out;
}