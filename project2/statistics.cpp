#include "statistics.h"
#include <sstream>
#include <iostream>
#include <numeric>
#include <algorithm>

template<typename T>
statistics<T>::statistics(/* args */) {
  seq = std::vector<T>();
  mode = std::unordered_map<T, unsigned int>();
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
double statistics<T>::get_sum() const {
  if (not is_ready(__func__)) {
    return 0.0;
  }
  else {
    return std::accumulate(seq.begin(), seq.end(), 0.0);
  }
}

template<typename T>
double statistics<T>::get_mean() const {
  if (not is_ready(__func__)) {
    return 0.0;
  }
  else {
    return 1.0 * (get_sum() / seq.size());
  }
}

template<typename T>
double statistics<T>::get_STD() const {
  if (not is_ready(__func__)) {
    return 0.0;
  }
  else {
    double std = std::sqrt(get_variance());
    return std;
  }
}

template<typename T>
double statistics<T>::get_variance() const {
  if (not is_ready(__func__)) {
    return 0.0;
  }
  else {
    double mean = get_mean();
    std::vector<T> seq_diff(seq.size());
    //Get the difference between each element in the original vector
    // And store it into a new container
    std::transform(seq.begin(), seq.end(), seq_diff.begin(),
                    [mean](T x) {return x - mean; });
    double variance =
      std::inner_product(seq_diff.begin(), seq_diff.end(),
                        seq_diff.begin(), 0.0) / seq.size();
    return variance;
  }
}

template<typename T>
T statistics<T>::get_mode() const {
  if (not is_ready(__func__)) {
    return (T)0.0;
  }
  else {
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
  if (vec.size() % 2 == 0) {
    const auto median_it1 = vec.begin() + vec.size() / 2 -1;
    const auto median_it2 = vec.begin() + vec.size() / 2;

    std::nth_element(vec.begin(), median_it1, vec.end());
    const auto e1 = *median_it1;

    std::nth_element(vec.begin(), median_it2, vec.size());
    const auto e2 = *median_it2;

    return (e1 + e2) / 2;
  }
  else {
    const auto median_it = vec.begin() + vec.size() / 2;
    std::nth_element(vec.begin(), median_it, vec.size());
    return *median_it;
  }
}

template<typename T>
std::vector<T> statistics<T>::get_quartiles() const {
  std::vector<T> sorted(seq.size());
  std::sort(seq.begin(), seq.end(), sorted);
  //Get the median for the second quartile. Then
  //Get the find the median between the median and the min for the 1st quartile
  //Get the median between the median and the max for the 3rd quartile
}

template<typename T>
bool statistics<T>::is_ready(const std::string func_name) const {
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
  return ss.str();
}

