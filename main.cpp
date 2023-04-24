#include <iostream>

#include "TapeSorter.h"
#include "TapeImpl.h"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: "  << argv[0] << " <input file> <output file>\n";
    return 1;
  }

  std::string input = argv[1];
  std::string output = argv[2];

  try
  {
    TapeConfigurator configurator;
    TapeImpl input_tape(input, configurator);
    TapeImpl output_tape(output, configurator);

    auto tape_factory = [&configurator](const std::string& file){
                          return std::make_unique<TapeImpl>(file, configurator);
    };

    TapeSorter sorter(input_tape, output_tape, configurator.get_memory_limit(),
                      tape_factory);
    sorter.sort();

    if (sorter.is_sorted())
      std::cout << "The output tape is sorted.\n";
    else
      std::cout << "The output tape is not sorted.\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
