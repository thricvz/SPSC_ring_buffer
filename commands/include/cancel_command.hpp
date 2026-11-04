#ifndef CANCEL_COMMAND_H
#define CANCEL_COMMAND_H

#include "command.hpp"

class cancel_command : public command {
  public:
    cancel_command() = default;
    
    virtual void execute(log_to_file_func , std::ofstream& file) override;
};

#endif

