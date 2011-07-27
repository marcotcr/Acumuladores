// Copyright (c) 2011 by  Marco Túlio Ribeiro <marcotcr@gmail.com>,
//                        Tiago Alves Macambira <tmacam@burocrata.org>
// All rights reserved.
//
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

// Implementation of counters.

#include "./counter.h";
#include <iostream>

Counter::Counter(const std::string& name, int value,
CounterManager* counter_manager): name_(name), value_(value),
counter_manager_(counter_manager) {
}

void Counter::Increment(int i) {
  value_ += i;
  counter_manager_->IncrementCounter(name_, i);
}

int Counter::value() {
  return value_;
}


CounterManager::CounterManager() {
}

void CounterManager::IncrementCounter(const std::string& name, int value) {
  if (counters_.count(name) <= 0) {
    counters_[name] = 0;
  }
  counters_[name] += value;
}

void CounterManager::OutputAndResetAllCounters(
RenderoCounterGroup* output_group) {
  std::tr1::unordered_map<std::string, int>::iterator it;
  for (it = counters_.begin(); it != counters_.end(); ++it) {
    if (it->second != 0) {
      RenderoCounter* temp_counter;
      temp_counter = output_group->add_counters();
      temp_counter->set_name(it->first);
      temp_counter->set_value(it->second);
      counters_[it->first] = 0;
    }
  }
}

int CounterManager::counter(const std::string& name) {
  if (counters_.count(name) <=0) {
    return -1;
  } else {
    return counters_[name];
  }
}

CounterAgregator::CounterAgregator() {
}

int CounterAgregator::counter(const std::string& name) {
  if (counters_.count(name) <=0) {
    return -1;
  } else {
    return counters_[name];
  }
}

void CounterAgregator::UpdateCounters(
const RenderoCounterGroup& counter_group) {
  for (int i = 0; i < counter_group.counters_size(); ++i) {
    const RenderoCounter& temp_counter = counter_group.counters(i);
    if (counters_.count(temp_counter.name()) <= 0) {
      counters_[temp_counter.name()] = 0;
    }
    counters_[temp_counter.name()] += temp_counter.value();
  }
}
