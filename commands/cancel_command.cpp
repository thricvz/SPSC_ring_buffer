#include "include/cancel_command.hpp"
#include <iostream>
#include <string>


void cancel_command::execute(log_to_file_func write_to_file , std::ofstream& file) {
  write_to_file("I guess it ends here\n", file);
  file.close(); 
};


