#include "statistics.h"
#include <sstream>
#include <iostream>
#include <numeric>
statistics::statistics(/* args */) {
  seq = std::vector<double>();
}

statistics::~statistics() {
  //STL containers destroy themselves automatically
  //In class destructors
}

void
statistics::enqueue(double x) {
  seq.push_back(x);
}

double
statistics::get_sum() const {
  if (not is_ready(__func__))
  {
    return 0.0;
  }
  else {
    return std::accumulate(seq.begin(), seq.end(), 0.0);
  }
}

double
statistics::get_mean() const {
  if (not is_ready(__func__)) {
    return 0.0;
  }
  else {
    return 1.0 * (get_sum() / seq.size());
  }
}

double
statistics::get_STD() const {
  if (not is_ready(__func__)) {
    return 0.0;
  }
  else {
    double std = std::sqrt(get_variance());
    return std;
  }
}

double
statistics::get_variance() const {
  if (not is_ready(__func__)) {
    return 0.0;
  }
  else {
    double mean = get_mean();
    std::vector<double> seq_diff(seq.size());
    //Get the difference between each element in the original vector
    // And store it into a new container
    std::transform(seq.begin(), seq.end(), seq_diff.begin(),
                    [mean](double x) {return x - mean; });
    double variance =
      std::inner_product(seq_diff.begin(), seq_diff.end(),
                        seq_diff.begin(), 0.0) / seq.size();
    return variance;
  }
}

bool
statistics::is_ready(const std::string func_name) const {
  if (not seq.empty()) {
    return true;
  }
  else {
    std::cout << " Cannot " << func_name << "!"
              << "Sequence is empty!" << std::endl;
    return false;
  }
}

std::string
statistics::to_string(const int start, const int stop) const {
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
  return ss.str();
}

