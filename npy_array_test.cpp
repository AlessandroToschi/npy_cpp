#define DEBUG 0

#include "npy_array.h"
#include <gtest/gtest.h>

TEST(NPYArrayTest, ConstructorPathStringTest)
{
    EXPECT_THROW(npy_array<float>{""}, npy_array_exception);
    EXPECT_THROW(npy_array<float>{nullptr}, std::logic_error);
    EXPECT_THROW(npy_array<float>{"dfgdfgdfgfd"}, npy_array_exception);
    
    EXPECT_NO_THROW(npy_array<float>{"./test/archive.npy"});

    try
    {
        npy_array<float>{""};
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.get_exception_type(), npy_array_exception_type::input_output_error);
    }

    try
    {
        npy_array<float>{"dfgdfgdfg"};
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.get_exception_type(), npy_array_exception_type::input_output_error);
    }
}

TEST(NPYArrayTest, ConstructorMagicStringTest)
{
    try
    {
        npy_array<float>{"./test/invalid_magic_string.npy"};
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.get_exception_type(), npy_array_exception_type::invalid_magic_string);
    }
}

TEST(NPYArrayTest, ConstructorVersionTest)
{
    EXPECT_NO_THROW(npy_array<float>{"./test/archive.npy"});
    EXPECT_NO_THROW(npy_array<float>{"./test/version_2.npy"});
    EXPECT_THROW(npy_array<float>{"./test/version_3.npy"}, npy_array_exception);
    EXPECT_THROW(npy_array<float>{"./test/version_0.npy"}, npy_array_exception);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}