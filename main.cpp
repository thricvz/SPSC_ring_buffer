#include "include/spsc_ring_buffer.hpp"
#include <iostream>
#include <cstdint>
#include <thread>
#include <format>

spsc_ring_buffer<int, 5> RING_BUFFER ;

void produce_sequence() {
  for (auto i{0}; i < 80; i++ ){
    RING_BUFFER.produce(i);
  }

}; 

void consumer_sequence() {
  while(true) {
    auto element = std::move(RING_BUFFER.consume());
    std::cout << std::format("New product {}\n", element);
  }
}; 

int main() {
  std::cout << "LAUNCHING EXPERIMENT\n";
  std::thread producer_thread(produce_sequence);

  std::thread consumer_thread(consumer_sequence);
  producer_thread.join();
  consumer_thread.join();

  return 0;
}


