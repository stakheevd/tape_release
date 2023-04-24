#pragma once

#include <string>
#include <fstream>

class TapeConfigurator
{
public:
  explicit TapeConfigurator(const std::string& fliename = "config.txt");

  int32_t get_memory_limit() const;
  int32_t get_read_delay() const;
  int32_t get_write_delay() const;
  int32_t get_move_delay() const;

private:
  int32_t memory_limit = 1048576;
  int32_t read_delay = 0;
  int32_t write_delay = 0;
  int32_t move_delay = 0;
};
