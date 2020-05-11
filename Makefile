BOOST_PATH = /usr/local
BOOST_INCLUDE_PATH = $(BOOST_PATH)/include/
BOOST_LIB_PATH = $(BOOST_PATH)/lib/

INCLUDE_PATH = ./include
SRC_INCLUDE_PATH = ./src
SRC_PATH = ./src
SRC_TEST_PATH = ./src/test

SRC_FILES = $(wildcard $(SRC_PATH)/*.cpp)
SRC_TEST_FILES = $(wildcard $(SRC_TEST_PATH)/*.cpp)

CXX_FLAGS=-g -O0 --std=c++11 -Wall -Wpedantic

INCLUDES = -I $(INCLUDE_PATH) -I $(SRC_INCLUDE_PATH) -I $(BOOST_INCLUDE_PATH)

all: shared_lib
	echo $(SRC_FILES)
	echo $(SRC_TEST_FILES)
	#g++ $(CXX_FLAGS) -I /usr/local/include/ main.cpp -o main -L /usr/local/lib/ -lboost_regex

shared_lib: $(SRC_FILES)
	echo "icao"

$(SRC_FILES): %.cpp : %.c
	echo "$@, $<"

%.cpp:
	echo "$@"

run_test:
	g++ $(CXX_FLAGS) -I /usr/local/include/ npy_array_test.cpp -o ./test/npy_array_test -L /usr/local/lib/ -lgtest -lboost_regex
	./test/npy_array_test 

.PHONY: clean
clean:
	rm -rf ./lib/* ./build/* ./bin/*