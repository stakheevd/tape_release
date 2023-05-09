#pragma once
#include <cstdint>

class TapeInterface
{
public:
  virtual void write(int32_t value) = 0;
  virtual int32_t read() = 0;
  virtual void rewind() = 0;
  virtual void move_forward() = 0;
  virtual void move_backward() = 0;
  virtual void open() = 0;
  virtual void close() = 0;
  virtual bool eof() = 0;
  virtual void reset_flags() = 0;

  virtual ~TapeInterface() noexcept = default;
};
