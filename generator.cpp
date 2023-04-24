#include <random>
#include <fstream>

int main(int argc, char* argv[])
{
  if (argc < 2)
    return 1;

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<int> dist(INT32_MIN, INT32_MAX);

  int number_of_elements = std::stoi(argv[1]);
  std::ofstream numbers_stream("numbers_" + std::string(argv[1]) + ".txt", std::ios::trunc);

  for (int i = 0; i < number_of_elements; i++)
  {
    numbers_stream << dist(rng) << " ";
  }

  numbers_stream.close();
}