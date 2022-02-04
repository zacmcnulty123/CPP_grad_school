#include "statistics.h"
#include <sstream>
#include <iostream>
#include <numeric>
#include <algorithm>
template<typename T>
statistics<T>::statistics(/* args */) {
  //Sequence to do calculations on
  seq = std::vector<T>();
  //easily keep track of the mode element
  mode = std::map<T, unsigned int>();
}

template<typename T>
statistics<T>::statistics(std::vector<T> vec) {
  seq = vec;
  for (const T val : vec) {
    mode[val]++;
  }
}

template<typename T>
statistics<T>::~statistics() {
  //STL containers destroy themselves automatically
  //In class destructors
}

template <typename T>
void statistics<T>::enqueue(T x) {
  seq.push_back(x);
  mode[x]++;
}

template<typename T>
double statistics<T>::get_mean() const {
  //Ensure elements exist
  if (not is_ready(__func__)) {
    return 0.0;
  }
  else {
    //Mean = Sum of all elements / number of elements
    return 1.0 * (get_sum() / seq.size());
  }
}

template<typename T>
double statistics<T>::get_STD() const {
  if (not is_ready(__func__)) {
    return 0.0;
  }
  else if (seq.size() == 1) {
    std::cout << "Cannot! " << __func__
      << " at least two data points needed!" << std::endl;
    return 0.0;
  }
  else {
    // Standard Deviation is the sqrt of the variance
    double std = std::sqrt(get_variance());
    return std;
  }
}

template<typename T>
double statistics<T>::get_sum() const {
  if (not is_ready(__func__)) {
    return 0.0;
  }
  else {
    //std::accumulate is a good way for getting the sum
    //set of values
    return std::accumulate(seq.begin(), seq.end(), 0.0);
  }
}

template<typename T>
double statistics<T>::get_variance() const {
  if (not is_ready(__func__)) {
    return 0.0;
  }
  else if (seq.size() == 1) {
    std::cout << "Cannot! " << __func__
      << " at least two data points needed!" << std::endl;
    return 0.0;
  }
  else {
    //Reference for variance calculation:
    //https://www.mathsisfun.com/data/standard-deviation.html
    double mean = get_mean();
    std::vector<T> seq_diff(seq.size());
    //Get the difference between each element in the original vector
    // And store it into a new container
    // for each x_i: (x_i - mean)
    std::transform(seq.begin(), seq.end(), seq_diff.begin(),
                    [mean](T x) {return x - mean;});
    //Need to multiply vector elements by themselves and divide by the size
    //This is sample variance given by N - 1. Not population variance
    double variance =
      std::inner_product(seq_diff.begin(), seq_diff.end(),
                        seq_diff.begin(), 0.0) / (seq.size() - 1);
    return variance;
  }
}

template<typename T>
T statistics<T>::get_mode() const {
  if (not is_ready(__func__)) {
    return (T)0.0;
  }
  else {
    // Quickly get the mode without counting each element
    auto tmp =
      std::max_element(mode.begin(), mode.end(),
            [](const std::pair<T, unsigned int> & x,
              const std::pair<T, unsigned int> & y)->bool 
              {return x.second < y.second;});
    return tmp->first;
  }
}

template<typename T>
T statistics<T>::get_median(std::vector<T> vec) const {
  //Want to do this method on a sorted array for simplicity
  //Need to handle even and odd length sequences
  std::vector<T> sorted = vec;
  std::sort(sorted.begin(), sorted.end());
  if (vec.size() % 2 == 0) {
    const auto median_l = sorted.size() / 2 - 1;
    const auto median_r = sorted.size() / 2;
    return (sorted[(median_l + median_r) / 2]);
  }
  else {
    return (sorted[sorted.size()/2]);
  }
}

template<typename T>
T statistics<T>::get_median() const {
  //This is the public function that makes it easier for
  //The users of the class
  if (not is_ready(__func__)) {
    return (T) 0.0;
  }
  else {
    return get_median(seq);
  }
}

template<typename T>
std::map<std::string, T> statistics<T>::get_quartiles() const {
  if (not is_ready(__func__)) {
    return std::map<std::string, T>();
  }
  else if (seq.size() == 1) {
    std::map<std::string, T> ret;
    ret["min"] = seq[0];
    ret["q1"] = seq[0];
    ret["q2"] = seq[0];
    ret["q3"] = seq[0];
    ret["max"] = seq[0];
    return ret;
  }
  else {
    //Getting quartiles are getting the min, max, and
    //finding the median of several ranges of the sorted vector
    std::map<std::string, T> ret;
    std::vector<T> sorted = seq;
    std::sort(sorted.begin(), sorted.end());
    std::vector<T> lower_half(sorted.begin(), sorted.begin() + sorted.size() / 2);
    std::vector<T> upper_half(sorted.begin() + sorted.size() / 2, sorted.end());
    ret["min"]= sorted[0];
    ret["q1"] = get_median(lower_half);
    ret["q2"] = get_median(sorted);
    ret["q3"] = get_median(upper_half);
    ret["max"] = sorted[sorted.size()-1];
    return ret;
  }
}

template<typename T>
std::string statistics<T>::get_descriptive_statistics() const {
  if (not is_ready(__func__)) {
  return std::string();
  }
  else {
    //The descriptive statistics are a combination of
    //the several functions in the class.
    std::stringstream cout;
    cout.precision(2);
    cout << "Average: " << std::fixed << get_mean() << std::endl;
    cout << "Mode: " << std::fixed << get_mode() << std::endl;
    cout << "Standard Deviation: " << get_STD() << std::endl;
    cout << "Variance: " << get_variance() << std::endl;
    cout << "Quartiles\n" << get_quartiles() << std::endl;
    return cout.str();
  }
}

template<typename T>
bool statistics<T>::is_ready(const std::string func_name) const {
  //Main function for handling the empty class case.
  if (not seq.empty()) {
    return true;
  }
  else {
    std::cout << " Cannot " << func_name << "!"
              << " Sequence is empty!" << std::endl;
    return false;
  }
}

template<typename T>
std::string statistics<T>::to_string(const int start, const int stop) const {
  if(not is_ready(__func__)) {
    return std::string();
  }
  int end = 0;
  int beg = start;
  if (stop < 0) {
    end = seq.size();
  }
  else if (stop < start) {
    std::cout << __func__ << " Invalid parameters!" <<
    " start: " << std::to_string(start) <<
    " is greater than stop: " << std::to_string(stop) << std::endl;
    return std::string();
  }
  else if (seq.size() < stop) {
    std::cout << __func__ << "Invalid parameters!"
    << " stop: " << std::to_string(stop) << " is greater than that size"
    << "of the sequence: " << std::to_string(seq.size()) << std::endl;
  }
  else {
    end = stop;
  }
  std::ostringstream ss;
  const char* separator = "";
  for (int i = start; i < end; i++) {
    ss << separator << seq[i];
    separator = ",";
  }
  ss << std::endl << "Sequence Size: " << seq.size() << std::endl;
  return ss.str();
}