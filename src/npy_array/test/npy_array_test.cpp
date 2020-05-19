#define DEBUG 0

#include "npy_array/npy_array.h"
#include <gtest/gtest.h>



TEST(NPYArrayTest, ConstructorPathStringTest)
{
    try
    {
        npy_array<float>{""};
        FAIL();
    }
    catch(const npy_array_exception& npy_exception)
    {
        EXPECT_EQ(npy_exception.exception_type(), npy_array_exception_type::input_output_error);
    }

    try
    {
        npy_array<float>{"/test/ereregfdgdfgdgdfgfdgfgdgfdfggdf.npy"};
        FAIL();
    }
    catch(const npy_array_exception& npy_exception)
    {
        EXPECT_EQ(npy_exception.exception_type(), npy_array_exception_type::input_output_error);
    }
}    

TEST(NPYArrayTest, ConstructorMagicStringTest)
{
    try
    {
        npy_array<float>{"./test_resources/invalid_magic_string.npy"};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::invalid_magic_string);
    }

    try
    {
        npy_array<float>{"./test_resources/carbon.png"};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::invalid_magic_string);
    }

    try
    {
        npy_array<float>{"./test_resources/archive.npy"};
        SUCCEED();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_NE(e.exception_type(), npy_array_exception_type::invalid_magic_string);
    }
}

TEST(NPYArrayTest, ConstructorMajorVersion)
{
    try
    {
        npy_array<float>{"./test_resources/version_2.npy"};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::unsupported_version);
    }

    try
    {
        npy_array<float>{"./test_resources/version_3.npy"};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::unsupported_version);
    }

    try
    {
        npy_array<float>{"./test_resources/version_0.npy"};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::unsupported_version);
    }

    try
    {
        npy_array<float>{"./test_resources/archive.npy"};
        SUCCEED();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_NE(e.exception_type(), npy_array_exception_type::unsupported_version);
    }
}

/*
TEST(NPYArrayTest, ConstructorMagicStringTest)
{
    try
    {
        npy_array<float>{"./test_resources/invalid_magic_string.npy"};
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::invalid_magic_string);
    }
}

TEST(NPYArrayTest, ConstructorVersionTest)
{
    EXPECT_NO_THROW(npy_array<float>{"./test_resources/archive.npy"});
    EXPECT_THROW(npy_array<float>{"./test_resources/version_2.npy"}, npy_array_exception);
    EXPECT_THROW(npy_array<float>{"./test_resources/version_3.npy"}, npy_array_exception);
    EXPECT_THROW(npy_array<float>{"./test_resources/version_0.npy"}, npy_array_exception);

    try
    {
        npy_array<float>{"./test_resources/version_2.npy"};
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::unsupported_version);
    }

    try
    {
        npy_array<float>{"./test_resources/version_3.npy"};
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::unsupported_version);
    }

    try
    {
        npy_array<float>{"./test_resources/version_0.npy"};
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::unsupported_version);
    }
}
*/
int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}