#ifndef COMMAND_H
#define COMMAND_H

#include <fstream>
#include <string>
#include <functional>


using log_to_file_func = std::function<void(const std::string&, std::ofstream&)>; 

class command {
  public:
    command() {


    };

    virtual void execute(log_to_file_func , std::ofstream& file) = 0;
};

#endif

