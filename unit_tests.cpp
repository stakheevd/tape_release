#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <filesystem>
#include "TapeSorter.h"
#include "TapeImpl.h"
#include "TapeConfigurator.h"

namespace fs = std::filesystem;

class TapeSorterTest : public ::testing::Test
{
protected:
    TapeSorterTest()
    {
    }

    void SetUp() override
    {
      for (const auto& entry : fs::directory_iterator("/tmp"))
      {
          if (entry.path().filename().string().find("temp_tape_") != std::string::npos)
          {
              fs::remove(entry.path());
          }
      }

      fs::remove("res.txt");
    }

    void TearDown() override
    {
      SetUp();
    }
};

TEST_F(TapeSorterTest, Sorting_0Element_Fail)
{
  TapeConfigurator configurator;
  TapeImpl input_tape("numbers_0.txt", configurator);
  TapeImpl output_tape("res.txt", configurator);
  auto tape_factory = [&configurator](const std::string& file)
  {
      return std::make_unique<TapeImpl>(file, configurator);
  };

  TapeSorter sorter(input_tape, output_tape, configurator.get_memory_limit(), tape_factory);
  ASSERT_THROW(sorter.sort(), std::runtime_error);
}

TEST_F(TapeSorterTest, Sorting_1Element_IsSorted)
{
  TapeConfigurator configurator;
  TapeImpl input_tape("numbers_1.txt", configurator);
  TapeImpl output_tape("res.txt", configurator);
  auto tape_factory = [&configurator](const std::string& file)
  {
      return std::make_unique<TapeImpl>(file, configurator);
  };

  TapeSorter sorter(input_tape, output_tape, configurator.get_memory_limit(), tape_factory);
  sorter.sort();

  ASSERT_TRUE(sorter.is_sorted());
}

TEST_F(TapeSorterTest, Sorting_100Element_IsSorted)
{
  TapeConfigurator configurator;
  TapeImpl input_tape("numbers_100.txt", configurator);
  TapeImpl output_tape("res.txt", configurator);
  auto tape_factory = [&configurator](const std::string& file)
  {
      return std::make_unique<TapeImpl>(file, configurator);
  };

  TapeSorter sorter(input_tape, output_tape, configurator.get_memory_limit(), tape_factory);
  sorter.sort();

  ASSERT_TRUE(sorter.is_sorted());
}

TEST_F(TapeSorterTest, Sorting_1000Element_IsSorted)
{
  TapeConfigurator configurator;
  TapeImpl input_tape("numbers_1000.txt", configurator);
  TapeImpl output_tape("res.txt", configurator);
  auto tape_factory = [&configurator](const std::string& file)
  {
      return std::make_unique<TapeImpl>(file, configurator);
  };

  TapeSorter sorter(input_tape, output_tape, configurator.get_memory_limit(), tape_factory);
  sorter.sort();

  ASSERT_TRUE(sorter.is_sorted());
}

TEST_F(TapeSorterTest, Sorting_10000Element_IsSorted)
{
  TapeConfigurator configurator;
  TapeImpl input_tape("numbers_10000.txt", configurator);
  TapeImpl output_tape("res.txt", configurator);
  auto tape_factory = [&configurator](const std::string& file)
  {
      return std::make_unique<TapeImpl>(file, configurator);
  };

  TapeSorter sorter(input_tape, output_tape, configurator.get_memory_limit(), tape_factory);
  sorter.sort();

  ASSERT_TRUE(sorter.is_sorted());
}

TEST_F(TapeSorterTest, Sorting_100000Element_IsSorted)
{
  TapeConfigurator configurator;
  TapeImpl input_tape("numbers_100000.txt", configurator);
  TapeImpl output_tape("res.txt", configurator);
  auto tape_factory = [&configurator](const std::string& file)
  {
      return std::make_unique<TapeImpl>(file, configurator);
  };

  TapeSorter sorter(input_tape, output_tape, configurator.get_memory_limit(), tape_factory);
  sorter.sort();

  ASSERT_TRUE(sorter.is_sorted());
}

TEST_F(TapeSorterTest, Sorting_1000000Element_IsSorted)
{
  TapeConfigurator configurator;
  TapeImpl input_tape("numbers_1000000.txt", configurator);
  TapeImpl output_tape("res.txt", configurator);
  auto tape_factory = [&configurator](const std::string& file)
  {
      return std::make_unique<TapeImpl>(file, configurator);
  };

  TapeSorter sorter(input_tape, output_tape, configurator.get_memory_limit(), tape_factory);
  sorter.sort();

  ASSERT_TRUE(sorter.is_sorted());
}

TEST_F(TapeSorterTest, Sorting_10000000Element_IsSorted)
{
  TapeConfigurator configurator;
  TapeImpl input_tape("numbers_10000000.txt", configurator);
  TapeImpl output_tape("res.txt", configurator);
  auto tape_factory = [&configurator](const std::string& file)
  {
      return std::make_unique<TapeImpl>(file, configurator);
  };

  TapeSorter sorter(input_tape, output_tape, configurator.get_memory_limit(), tape_factory);
  sorter.sort();

  ASSERT_TRUE(sorter.is_sorted());
}

TEST_F(TapeSorterTest, InvalidInput)
{
    TapeConfigurator configurator;
    TapeImpl input_tape("invalid_data.txt", configurator);
    TapeImpl output_tape("res.txt", configurator);
    auto tape_factory = [&configurator](const std::string& file)
    {
        return std::make_unique<TapeImpl>(file, configurator);
    };

    TapeSorter sorter(input_tape, output_tape, configurator.get_memory_limit(), tape_factory);
    ASSERT_THROW(sorter.sort(), std::runtime_error);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
