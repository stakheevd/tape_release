#include "TapeConfigurator.h"

TapeConfigurator::TapeConfigurator(const std::string& filename)
{
  std::ifstream config_file(filename);

  if (config_file.is_open())
  {
    config_file >> memory_limit;
    if (memory_limit < 5)
      throw std::runtime_error("Minimum memory size: 4 bytes");

    config_file >> read_delay;
    if (read_delay < 0)
      throw std::runtime_error("Read delay must not be negative");

    config_file >> write_delay;
    if (write_delay < 0)
      throw std::runtime_error("Write delay must not be negative");

    config_file >> move_delay;
    if (move_delay < 0)
      throw std::runtime_error("Move delay must not be negative");
  } 
}

int32_t TapeConfigurator::get_memory_limit() const
{
  return memory_limit;
}

int32_t TapeConfigurator::get_read_delay() const
{
  return read_delay;
}

int32_t TapeConfigurator::get_write_delay() const
{
  return write_delay;
}

int32_t TapeConfigurator::get_move_delay() const
{
  return move_delay;
}
