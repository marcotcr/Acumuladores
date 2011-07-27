#include <limits.h>
#include "counter.h"
#include <gtest/gtest.h>
#include <iostream>
#include <string.h>

TEST(CounterManagerTest, IncrementCounter) {
  CounterManager counter_manager;
  int counter_value;
  counter_manager.IncrementCounter("test_counter1", 1);
  counter_value = counter_manager.counter("test_counter1");
  EXPECT_EQ(1, counter_value);
  counter_manager.IncrementCounter("test_counter1", 3);
  counter_value = counter_manager.counter("test_counter1");
  EXPECT_EQ(4, counter_value);
}

TEST(CounterManagerTest, GetInvalidCounter) {
  CounterManager counter_manager;
  int counter_value;
  counter_value = counter_manager.counter("test_counter1");
  EXPECT_EQ(-1, counter_value);
}

TEST(CounterManagerTest, OutputAndResetAllCounters) {
  CounterManager counter_manager;
  counter_manager.IncrementCounter("test_counter1", 3);
  counter_manager.IncrementCounter("test_counter2", 1);
  RenderoCounterGroup output_counters;
  counter_manager.OutputAndResetAllCounters(&output_counters);
  int counter_value;
  counter_value = counter_manager.counter("test_counter1");
  EXPECT_EQ(0, counter_value);
  counter_value = counter_manager.counter("test_counter2");
  EXPECT_EQ(0, counter_value);
  //Test the output
  const RenderoCounter& temp_counter1 = output_counters.counters(0);
  const RenderoCounter& temp_counter2 = output_counters.counters(1);
  EXPECT_EQ("test_counter1", temp_counter1.name());
  EXPECT_EQ("test_counter2", temp_counter2.name());

  EXPECT_EQ(3, temp_counter1.value());
  EXPECT_EQ(1, temp_counter2.value());
}

TEST(CounterTest, Increment) {
  CounterManager counter_manager;
  Counter counter("test_counter1", 0, &counter_manager);
  counter.Increment(1);
  EXPECT_EQ(1, counter.value());
  EXPECT_EQ(1, counter_manager.counter("test_counter1"));
  counter.Increment(3);
  EXPECT_EQ(4, counter.value());
  EXPECT_EQ(4, counter_manager.counter("test_counter1"));
}

TEST(CounterAgregator, UpdateCounters) {
  CounterManager counter_manager1;
  CounterManager counter_manager2;
  counter_manager1.IncrementCounter("test_counter1", 1 );
  counter_manager1.IncrementCounter("test_counter2", 1);
  counter_manager2.IncrementCounter("test_counter1", 1 );
  counter_manager2.IncrementCounter("test_counter3", 1 );

  RenderoCounterGroup message1;
  counter_manager1.OutputAndResetAllCounters(&message1);
  RenderoCounterGroup message2;
  counter_manager2.OutputAndResetAllCounters(&message2);

  CounterAgregator counter_agregator;
  counter_agregator.UpdateCounters(message1);
  counter_agregator.UpdateCounters(message2);
  EXPECT_EQ(2, counter_agregator.counter("test_counter1"));
  EXPECT_EQ(1, counter_agregator.counter("test_counter2"));
  EXPECT_EQ(1, counter_agregator.counter("test_counter3"));

  counter_manager1.IncrementCounter("test_counter1", 3 );
  counter_manager1.IncrementCounter("test_counter2", 2);

  counter_manager2.IncrementCounter("test_counter1", 1 );
  counter_manager2.IncrementCounter("test_counter3", 2 );

  RenderoCounterGroup message3;
  counter_manager1.OutputAndResetAllCounters(&message3);
  RenderoCounterGroup message4;
  counter_manager2.OutputAndResetAllCounters(&message4);

  counter_agregator.UpdateCounters(message3);
  counter_agregator.UpdateCounters(message4);

  EXPECT_EQ(6, counter_agregator.counter("test_counter1"));
  EXPECT_EQ(3, counter_agregator.counter("test_counter2"));
  EXPECT_EQ(3, counter_agregator.counter("test_counter3"));


}
