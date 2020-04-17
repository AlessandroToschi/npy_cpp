CXX_FLAGS=-g -O0 --std=c++11 -Wall -Wpedantic

all:
	g++ $(CXX_FLAGS) main.cpp -o main