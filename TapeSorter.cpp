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
    temp_tapes.back()->close();
  }

  if (temp_tapes.size() == 1)
  {
    temp_tapes[0]->open();

    while (!temp_tapes[0]->eof())
    {
      int32_t value = temp_tapes[0]->read();

      if (temp_tapes[0]->eof())
        break;

      output_tape.write(value);
      temp_tapes[0]->move_forward();
      output_tape.move_forward();
    }

    output_tape.rewind();
    temp_tapes[0]->close();
  }
  else
  {
    while (temp_tapes.size() > 1)
    {
      std::vector<std::unique_ptr<TapeInterface>> merged_tapes;

      for (size_t i = 0; i < temp_tapes.size(); i += 2)
      {
        if (i + 1 < temp_tapes.size())
        {
          temp_tapes[i]->open();
          temp_tapes[i + 1]->open();

          if (temp_tapes.size() == 2)
          {
            merge(*temp_tapes[i], *temp_tapes[i + 1], output_tape);
            output_tape.rewind();
          }
          else
          {
            auto merged_tape  = tape_factory("/tmp/temp_tape_" + std::to_string(temp_tape_counter++) + ".txt");
            merged_tapes.push_back(std::move(merged_tape));
            merge(*temp_tapes[i], *temp_tapes[i + 1], *merged_tapes.back());

            merged_tapes.back()->rewind();
            merged_tapes.back()->close();
          }

          temp_tapes[i]->close();
          temp_tapes[i + 1]->close();
        }
        else
          merged_tapes.push_back(std::move(temp_tapes[i]));
      }

      temp_tapes = std::move(merged_tapes);
    }
  }

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

void TapeSorter::merge(TapeInterface& left_tape, TapeInterface& right_tape, TapeInterface& output_tape) 
{
  left_tape.rewind();
  right_tape.rewind();
  int32_t left_value = left_tape.read();
  int32_t right_value = right_tape.read();
  
  while (!left_tape.eof() && !right_tape.eof()) 
  {
    if (left_value < right_value) 
    {
      output_tape.write(left_value);

      left_tape.move_forward();
      if (!left_tape.eof())
        left_value = left_tape.read();
    }
    else
    {
      output_tape.write(right_value);

      right_tape.move_forward();
      if (!right_tape.eof())
        right_value = right_tape.read();
    }

    output_tape.move_forward();
  }

  while (!left_tape.eof()) 
  {
    output_tape.write(left_value);
    output_tape.move_forward();

    left_tape.move_forward();
    if (!left_tape.eof())
      left_value = left_tape.read();
  }

  while (!right_tape.eof())
  {
    output_tape.write(right_value);
    output_tape.move_forward();

    right_tape.move_forward();
    if (!right_tape.eof())
      right_value = right_tape.read();
  }
}
