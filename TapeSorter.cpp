#include "TapeSorter.h"

TapeSorter::TapeSorter(TapeInterface& in_tape, TapeInterface& out_tape, int32_t mem_limit,
                       std::function<std::unique_ptr<TapeInterface>(const std::string&)> tape_factory_func) :
  input_tape(in_tape),
  output_tape(out_tape),
  memory_limit(mem_limit),
  tape_factory(tape_factory_func)
{
}

void TapeSorter::sort()
{
  const int MAX_VALUES_IN_MEMORY = memory_limit / sizeof(int32_t);

  std::vector<std::unique_ptr<TapeInterface>> temp_tapes;
  input_tape.rewind();

  std::size_t temp_tape_counter = 0;

  std::vector<int32_t> buffer;
  buffer.reserve(MAX_VALUES_IN_MEMORY);

  while (!input_tape.eof()) 
  {
    buffer.clear();

    for (int i = 0; i < MAX_VALUES_IN_MEMORY && !input_tape.eof(); ++i) 
    {
      int32_t value = input_tape.read();

      if (input_tape.eof())
        break;

      buffer.push_back(value);
      input_tape.move_forward();
    }
    
    std::sort(std::begin(buffer), std::end(buffer));

    auto temp_tape = tape_factory("/tmp/temp_tape_" + std::to_string(temp_tape_counter++) + ".txt");
    temp_tapes.push_back(std::move(temp_tape));

    for (const auto& value : buffer)
    {
      temp_tapes.back()->write(value);
      temp_tapes.back()->move_forward();
    }

    temp_tapes.back()->rewind();
    //temp_tapes.back()->close();
  }

  merge(temp_tapes, output_tape);
  output_tape.rewind();
}

bool TapeSorter::is_sorted()
{
  if (output_tape.eof())
    return true;

  output_tape.rewind();

  int32_t prev_value = output_tape.read();
  output_tape.move_forward();

  while (!output_tape.eof())
  {
    int32_t current_value = output_tape.read();
    output_tape.move_forward();

    if (current_value < prev_value)
      return false;

    prev_value = current_value;
  }

  return true;
}

void TapeSorter::merge(std::vector<std::unique_ptr<TapeInterface>>& temp_tapes, TapeInterface& output_tape)
{
  std::priority_queue<std::pair<int32_t, TapeInterface*>,
                      std::vector<std::pair<int32_t, TapeInterface*>>, std::greater<>> pq;

  for (auto& tape : temp_tapes)
  {
    //tape->open();
    if (!tape->eof())
    {
      pq.push({tape->read(), tape.get()});
      tape->move_forward();
    }
    //tape->close();
  }

  while (!pq.empty())
  {
    auto [value, tape] = pq.top();
    pq.pop();

    output_tape.write(value);
    output_tape.move_forward();

    //tape->open();
    if (!tape->eof())
    {
      pq.push({tape->read(), tape});
      tape->move_forward();
    }
    //tape->close();
  }
}