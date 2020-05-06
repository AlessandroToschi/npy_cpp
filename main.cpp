#include <iostream>
#include <vector>
#include <algorithm>

#define DEBUG 1

#include "npy_array.h"


int main(int argc, char *argv[])
{
    //npy_array pippo{"./archive.npy"};

    std::vector<size_t> shape{1, 256, 32, 32};

    std::cout << "Before" << std::endl;
    std::cout << "Shape size: " << shape.size() << std::endl;
    std::for_each(shape.begin(), shape.end(), [](const size_t& i){std::cout << i << std::endl;});

    npy_array<float> pippo{"./archive.npy"};
    npy_array<float> gfgf{"./test/version_2.npy"};
    //pippo.do_something();
    //prova_move({1, 2, 3, 4});

    std::cout << "After" << std::endl;
    std::cout << "Shape size: " << shape.size() << std::endl;
    std::for_each(shape.begin(), shape.end(), [](const size_t& i){std::cout << i << std::endl;});

    return 0;
}