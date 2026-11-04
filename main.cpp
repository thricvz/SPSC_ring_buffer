#include <iostream>
#include <cstdint>
#include <memory>
#include <thread>
#include <format>

#include "commands/include/echo_command.hpp"
#include "commands/include/cancel_command.hpp"
#include "include/spsc_ring_buffer.hpp"

spsc_ring_buffer<std::unique_ptr<command>, 5> RING_BUFFER ;

void write_to_file(const std::string& description, std::ofstream& out) {
    out << description;
};

void produce_sequence() {
  std::string command;
  while (true) {

    std::cout << "Please enter a command:\n";
    std::cin >> command;
    
    if (command == "exit") {
      RING_BUFFER.produce(std::make_unique<cancel_command>());
      break;

    } else if (command == "echo") {
      std::string echo_msg;
      std::cout << "Write the message to be output: ";
      std::cin >> echo_msg;
    
      RING_BUFFER.produce(std::make_unique<echo_command>(echo_msg));
    }


  }

}; 

void consumer_sequence() {
  std::ofstream output_file("log.txt");

  while(true) {
    auto parsed_command = std::move(RING_BUFFER.consume());
    parsed_command->execute(write_to_file, output_file);
  }

  //file is closed by a command

}; 

int main() {
  std::cout << "LAUNCHING EXPERIMENT\n";
  std::thread producer_thread(produce_sequence);
  
  std::thread consumer_thread(consumer_sequence);
  producer_thread.join();
  consumer_thread.join();

  return 0;
}


