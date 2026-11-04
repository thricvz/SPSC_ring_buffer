#ifndef ECHO_COMMAND_H
#define ECHO_COMMAND_H

#include "command.hpp"

class echo_command: public command {
  public:
    echo_command(std::string msg) : command(), m_msg{std::move(msg)} {};


    virtual void execute(log_to_file_func , std::ofstream& file) override;

  private:
    std::string m_msg{};
};

#endif

