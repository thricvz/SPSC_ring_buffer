#include "include/echo_command.hpp"
#include <iostream>
#include <format>

void echo_command::execute(log_to_file_func write_to_file, std::ofstream& file) {
  write_to_file(std::format("Message from user: {}\n", m_msg), file);
};


