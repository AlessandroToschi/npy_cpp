CXX_FLAGS=-g -O0 --std=c++11 -Wall -Wpedantic

all:
	g++ $(CXX_FLAGS) main.cpp -o main

run_test:
	g++ $(CXX_FLAGS) -I /usr/local/include/ npy_array_test.cpp -o ./test/npy_array_test -L /usr/local/lib/ -lgtest
	./test/npy_array_test