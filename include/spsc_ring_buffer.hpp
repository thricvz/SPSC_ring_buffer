#ifndef SPSC_RING_BUFFER_H
#define SPSC_RING_BUFFER_H

#include <algorithm>
#include <climits>
#include <cstdint>
#include <atomic>
#include <iostream>
#include <mutex>


template<typename T, std::int32_t N_ELEMENTS >

class spsc_ring_buffer {
  public:
    spsc_ring_buffer() = default;

    void produce(const T& element) {

      while(consumer_producer_distance() > N_ELEMENTS) {
        ; // wait until we can write to memory without erasing
      }

      m_objects[wrap_index(m_producer_index)] = element;

      int32_t old_producer_index {m_producer_index};  
      int32_t new_producer_index {old_producer_index + 1};  
     

      while(!m_producer_index.compare_exchange_weak(old_producer_index, new_producer_index)) 
      {
        ;
      };

      check_for_index_overflow();
    };


    T consume() {
      while(m_consumer_index+1 == m_producer_index) {
        ; // wait
      }
      
      std::int32_t consumer_index_cpy {m_consumer_index};
      const std::int32_t new_consumer_index { m_consumer_index + 1 };

      T consumed_element = m_objects[wrap_index(new_consumer_index)]; 
      
      while(!m_consumer_index.compare_exchange_weak(consumer_index_cpy, new_consumer_index))
      {
        ;
      };

      return consumed_element;
    };

  private:
    void check_for_index_overflow() {
      //max integer accepted value
      if (m_producer_index + 1 != INT_MAX) 
        return ;
      
      const int32_t consumer_mapped_index { wrap_index(m_consumer_index)};
      
      //update atomically
      const int32_t new_consumer_index = wrap_index(m_consumer_index);
      const int32_t _consumer_producer_distance = consumer_producer_distance();

      m_consumer_index.store(wrap_index(m_consumer_index));
      m_consumer_index.store(m_consumer_index + _consumer_producer_distance);

    }

    std::int32_t wrap_index(std::int32_t index) const {
      return index % N_ELEMENTS;
    }

    std::int32_t consumer_producer_distance() {
      return m_producer_index - m_consumer_index;
    }

    T m_objects[N_ELEMENTS];
    std::atomic<std::int32_t> m_producer_index {0};
    std::atomic<std::int32_t> m_consumer_index {-1};
};

#endif

