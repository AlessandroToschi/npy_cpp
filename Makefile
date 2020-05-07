BOOST_PATH = /usr/local
BOOST_INCLUDE_PATH = $(BOOST_PATH)/include/
BOOST_LIB_PATH = $(BOOST_PATH)/lib/

CXX_FLAGS=-g -O0 --std=c++11 -Wall -Wpedantic

all:
	g++ $(CXX_FLAGS) -I /usr/local/include/ main.cpp -o main -L /usr/local/lib/ -lboost_regex

run_test:
	g++ $(CXX_FLAGS) -I /usr/local/include/ npy_array_test.cpp -o ./test/npy_array_test -L /usr/local/lib/ -lgtest -lboost_regex
	./test/npy_array_test 