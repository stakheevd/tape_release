all: build test
.PHONY: all build run test clean

build:
	clang++ main.cpp TapeConfigurator.cpp TapeImpl.cpp TapeSorter.cpp -o tape
	clang++ generator.cpp -o generator

run:
	rm -f /tmp/temp_tape_*
	./tape $(input_file) $(output_file)
	rm -f /tmp/temp_tape_*

test:
	clang++ generator.cpp -o generator
	clang++ -std=c++17 unit_tests.cpp TapeImpl.cpp TapeSorter.cpp TapeConfigurator.cpp -lgtest -lgtest_main -pthread -o unit_tests
	./generator 0
	./generator 1
	./generator 100
	./generator 1000
	./generator 10000
	./generator 100000
	./generator 1000000
	./generator 10000000
	echo "3 0 -12 4 12 -99 1 -35 4 pl 6 2 -32 1 8" > invalid_data.txt
	./unit_tests

clean:
	rm -f /tmp/temp_tape*
	rm -f numbers_*.txt
	rm -f invalid_data.txt
