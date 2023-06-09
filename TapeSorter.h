#pragma once

#include <vector>
#include <queue>
#include <functional>
#include <memory>
#include <algorithm>
#include <iostream>

#include "TapeInterface.h"

class TapeSorter
{
public:
  TapeSorter(TapeInterface& in_tape, TapeInterface& out_tape, int32_t mem_limit,
             std::function<std::unique_ptr<TapeInterface>(const std::string&)> tape_factory_func);
  void sort();
  bool is_sorted();

private:
  TapeInterface& input_tape;
  TapeInterface& output_tape;
  int32_t memory_limit;
  std::function<std::unique_ptr<TapeInterface>(const std::string&)> tape_factory;

  void merge(std::vector<std::unique_ptr<TapeInterface>>& temp_tapes, TapeInterface& output_tape);
};
