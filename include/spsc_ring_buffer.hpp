#ifndef SPSC_RING_BUFFER_H
#define SPSC_RING_BUFFER_H

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <mutex>

template<typename T, std::uint32_t N_ELEMENTS >

class spsc_ring_buffer {
  public:
    spsc_ring_buffer() = default;
    
    void produce(const T& element) {
      
      while(m_producer_index == m_consumer_index) {
        ; // wait until we can write to memory without erasing
      }

      std::scoped_lock lock(m_rw_mtx);

      m_objects[m_producer_index] = element;
      m_producer_index = wrap_index(++m_producer_index);
      
      INITIAL_WRITE_FLAG ? 
        INITIAL_WRITE_FLAG = false:
        INITIAL_WRITE_FLAG = true;

    };

    /*void produce(T&& element) {

    };*/

    T consume() {
      while(wrap_index(m_consumer_index+1) == m_producer_index) {
        ; // wait
      }

      std::scoped_lock lock(m_rw_mtx);

      T consumed_element = m_objects[m_consumer_index]; 
      m_consumer_index = wrap_index(++m_consumer_index);

      return consumed_element;
    };

  private:
    std::int32_t wrap_index(std::int32_t index) const {
      return index % N_ELEMENTS;
    }

    T m_objects[N_ELEMENTS];
    std::mutex m_rw_mtx;

    bool INITIAL_WRITE_FLAG {true};
    std::int32_t m_producer_index {0};
    std::int32_t m_consumer_index {-1};
};

#endif

