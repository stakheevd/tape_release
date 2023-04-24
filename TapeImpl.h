#pragma once

#include <thread>
#include <chrono>

#include "TapeInterface.h"
#include "TapeConfigurator.h"

class TapeImpl : public TapeInterface
{
public:
  TapeImpl(const std::string& file, TapeConfigurator& config);

  TapeImpl(const TapeImpl&) = delete;
  TapeImpl& operator=(const TapeImpl&) = delete;

  TapeImpl(TapeImpl&&) noexcept;
  TapeImpl& operator=(TapeImpl&&) noexcept;

  void write(int32_t value) override;
  int32_t read() override;
  void rewind() override;
  void move_forward() override;
  void move_backward() override;
  void open() override;
  void close() override;
  bool eof() override;

  ~TapeImpl() noexcept override;

private:
  std::fstream file_stream;
  std::string filename;
  TapeConfigurator& configurator;
};
