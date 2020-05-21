SHELL=/bin/bash

BOOST_PATH = /usr/local
BOOST_INCLUDE_PATH = $(BOOST_PATH)/include/
BOOST_LIB_PATH = $(BOOST_PATH)/lib/

INCLUDE_PATH = include
SRC_INCLUDE_PATH = src
SRC_PATH = src
SRC_TEST_PATH = src/test

SRC_FILES := $(shell find $(SRC_PATH)/ ! -name "*_test.cpp" -name "*.cpp")
SRC_TEST_FILES = $(shell find $(SRC_PATH)/ -name "*_test.cpp")

OBJECT_FILES := $(addprefix build/, $(SRC_FILES:%.cpp=%.o))
TEST_OBJECT_FILES := $(SRC_TEST_FILES:%.cpp=%.o)

CXX = g++
CXXFLAGS= -g -c -fPIC -O3 -march=native --std=c++11 -Wall -Wpedantic

INCLUDES = -I $(INCLUDE_PATH) -I $(SRC_INCLUDE_PATH) -I $(BOOST_INCLUDE_PATH)

all: make_dir shared_lib test

make_dir:
	@mkdir -p ./lib
	@mkdir -p ./build
	@mkdir -p ./bin

shared_lib: $(OBJECT_FILES)
	$(CXX) -shared $(OBJECT_FILES) -o lib/libnpy_array.so -L /usr/local/lib -lboost_regex

build/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@

.PHONY: test
test: shared_lib $(TEST_OBJECT_FILES)
	#@cp -r test_resources bin/

%_test.o: %_test.cpp
	@echo $(@F)
	$(CXX) -g --std=c++11 -Wall -Wpedantic $(INCLUDES) $< -o bin/$(basename $(@F)) -L /usr/local/lib/ -lgtest -lboost_regex -L lib -lnpy_array -Wl,-rpath=./lib




.PHONY: clean
clean:
	rm -rf ./lib/* ./build/* ./bin/*