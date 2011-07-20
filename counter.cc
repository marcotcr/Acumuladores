#ifndef COUNTER_H_
#define COUNTER_H_

#include <iostream>
#include <string>
#include <tr1/unordered_map>
#include "counter.pb.h"

using namespace std;
using namespace std::tr1;

// We don't need this yet.
//class Counter {
// public:
//  Counter(string name, int value): name_(name), value_(value) { }
//  void Increment(int i) { value_ += i; }
// private:
//  string name_;
//  int value_;
//};

class CounterManager {
 public:
  CounterManager() {}
  void IncrementCounter(string name, int value) {
    if (counters_.count(name) <= 0) {
      counters_[name] = 0;
    }
    counters_[name] += value;
  }
 RenderoCounterGroup OutputAndResetAllCounters() {
   RenderoCounterGroup output;
   unordered_map<string, int>::iterator it;
   for (it = counters_.begin(); it != counters_.end(); ++it) {
     if (it->second != 0) {
       RenderoCounter* temp_counter;
       temp_counter = output.add_counters();
       temp_counter->set_name(it->first);
       temp_counter->set_value(it->second);
       counters_[it->first] = 0;
     }
   }
   return output;
 }
 private:
  unordered_map<string, int> counters_;
};

class CounterAgregator {
 public:
  void UpdateCounters(RenderoCounterGroup counter_group) {
    for (int i = 0; i < counter_group.counters_size(); ++i) {
      const RenderoCounter& temp_counter = counter_group.counters(i);
      if (counters_.count(temp_counter.name()) <= 0) {
        counters_[temp_counter.name()] = 0;
      }
      counters_[temp_counter.name()] += temp_counter.value();
    }
  }
  void PrintCounters() {
    unordered_map<string, int>::iterator it;
    for (it = counters_.begin(); it != counters_.end(); ++it) {
      cout << it->first<<" "<<it->second<<endl;
    }
  }
 private:
  unordered_map<string, int> counters_;
  
};

#endif /* COUNTER_H_ */
