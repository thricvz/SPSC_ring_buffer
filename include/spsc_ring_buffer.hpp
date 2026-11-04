#ifndef SPSC_RING_BUFFER_H
#define SPSC_RING_BUFFER_H

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <climits>
#include <mutex>

template<typename T, std::int32_t N_ELEMENTS >

//weird bug that can happen when the int32_t wrapper overflows
//
class spsc_ring_buffer {
  public:
    spsc_ring_buffer() = default;

    void produce(const T& element) {

      while(consumer_producer_distance() > N_ELEMENTS) {
        ; // wait until we can write to memory without erasing
      }

      std::scoped_lock lock(m_rw_mtx);

      m_objects[wrap_index(m_producer_index)] = element;
      m_producer_index++;  
      
      check_for_index_overflow();

    };

    void produce(T&& element) {

      while(consumer_producer_distance() > N_ELEMENTS) {
        ; // wait until we can write to memory without erasing
      }

      std::scoped_lock lock(m_rw_mtx);

      m_objects[wrap_index(m_producer_index)] = std::move(element);
      m_producer_index++;  
      
      check_for_index_overflow();
    };

    T consume() {
      while(m_consumer_index+1 == m_producer_index) {
        ; // wait
      }

      std::scoped_lock lock(m_rw_mtx);

      m_consumer_index++;
      T consumed_element = std::move(m_objects[wrap_index(m_consumer_index)]); 

      return consumed_element;
    };

  private:
    void check_for_index_overflow() {
      //max integer accepted value
      if (m_producer_index + 1 != INT_MAX) 
        return ;
      
      const int32_t distance_producer_consumer = consumer_producer_distance();
      const int32_t consumer_spot { wrap_index(m_consumer_index)};

      m_producer_index = consumer_spot + distance_producer_consumer;  
      m_consumer_index = wrap_index(m_consumer_index);
    }

    std::int32_t wrap_index(std::int32_t index) const {
      return index % N_ELEMENTS;
    }

    std::int32_t consumer_producer_distance() {
      return m_producer_index - m_consumer_index;
    }

    T m_objects[N_ELEMENTS];
    std::mutex m_rw_mtx;
    std::int32_t m_producer_index {0};
    std::int32_t m_consumer_index {-1};
};

#endif

