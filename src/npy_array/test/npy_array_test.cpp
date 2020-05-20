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

TEST(NPYArrayTest, ConstructorHeaderParsing)
{
    try
    {
        npy_array<float>{"./test_resources/null_header.npy"};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::ill_formed_header);
    }

    try
    {
        npy_array<float>{"./test_resources/empty_header.npy"};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::ill_formed_header);
    }

    try
    {
        npy_array<float>{"./test_resources/fake_dtype.npy"};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::ill_formed_header);
    }

    try
    {
        npy_array<float>{"./test_resources/fake_descr.npy"};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::ill_formed_header);
    }

    try
    {
        npy_array<float>{"./test_resources/fake_fortran_order.npy"};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::ill_formed_header);
    }

    try
    {
        npy_array<float>{"./test_resources/fake_fortran_value.npy"};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::ill_formed_header);
    }

    try
    {
        npy_array<float>{"./test_resources/fake_shape.npy"};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::ill_formed_header);
    }

    try
    {
        npy_array<float>{"./test_resources/fake_shape_2.npy"};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::ill_formed_header);
    }

    try
    {
        npy_array<float>{"./test_resources/fake_shape_3.npy"};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::ill_formed_header);
    }

    EXPECT_NO_THROW(npy_array<long>{"./test_resources/fake_shape_4.npy"});
}

TEST(NPYArrayTest, StringConstructor)
{   
    EXPECT_NO_THROW(npy_array<float>{"./test_resources/archive.npy"});
    npy_array<float> array{"./test_resources/archive.npy"};
    std::vector<size_t> shape{1, 256, 13, 13};
    EXPECT_EQ(array.byte_size(), sizeof(float) * 256 * 13 * 13);
    EXPECT_EQ(array.dtype(), npy_dtype::float_32());
    EXPECT_EQ(array.fortran_order(), false);
    EXPECT_EQ(array.shape(), shape);
}

TEST(NPYArrayTest, CopyShapeConstructor)
{   
    std::vector<size_t> shape{1, 256, 13, 13};
    npy_array<float> array{shape};

    EXPECT_EQ(array.byte_size(), sizeof(float) * 256 * 13 * 13);
    EXPECT_EQ(array.dtype(), npy_dtype::float_32());
    EXPECT_EQ(array.fortran_order(), false);
    EXPECT_EQ(array.shape(), shape);

    EXPECT_EQ(shape.size(), 4);

    EXPECT_THROW(npy_array<std::bitset<3>>{shape}, npy_array_exception);
}

TEST(NPYArrayTest, MoveShapeConstructor)
{   
    std::vector<size_t> shape{1, 256, 13, 13};
    npy_array<float> array{std::move(shape)};

    EXPECT_EQ(array.byte_size(), sizeof(float) * 256 * 13 * 13);
    EXPECT_EQ(array.dtype(), npy_dtype::float_32());
    EXPECT_EQ(array.fortran_order(), false);
    EXPECT_NE(array.shape(), shape);

    EXPECT_EQ(shape.size(), 0);

    EXPECT_THROW(npy_array<std::bitset<3>>{shape}, npy_array_exception);
}

TEST(NPYArrayTest, ShapeListConstructor)
{   
    npy_array<float> array{{1, 256, 13, 13}};

    EXPECT_EQ(array.byte_size(), sizeof(float) * 256 * 13 * 13);
    EXPECT_EQ(array.dtype(), npy_dtype::float_32());
    EXPECT_EQ(array.fortran_order(), false);
}

TEST(NPYArrayTest, ShapeDataCopyConstructor)
{
    std::vector<size_t> shape{{2, 3}};
    std::vector<size_t> wrong_shape{{2, 3, 4}};
    std::vector<uint8_t> data{{1, 2, 3, 4, 5, 6}};
    std::vector<uint8_t> wrong_data{{1, 2, 3, 4}};

    npy_array<uint8_t> arr{shape, data};

    EXPECT_EQ(arr.byte_size(), sizeof(uint8_t) * 2 * 3);
    EXPECT_EQ(arr.dtype(), npy_dtype::uint_8());
    EXPECT_EQ(arr.fortran_order(), false);
    EXPECT_EQ(arr.shape(), shape);
    EXPECT_EQ(arr.size(), 2 * 3);

    try
    {
        npy_array<uint8_t>{wrong_shape, data};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::unmatched_shape_data);
    }

    try
    {
        npy_array<uint8_t>{shape, wrong_data};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::unmatched_shape_data);
    }
    
    try
    {
        npy_array<uint8_t>{wrong_shape, wrong_data};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::unmatched_shape_data);
    }
}

TEST(NPYArrayTest, ShapeDataMoveConstructor)
{
    std::vector<size_t> shape{{2, 3}};
    std::vector<size_t> wrong_shape{{2, 3, 4}};
    std::vector<uint8_t> data{{1, 2, 3, 4, 5, 6}};
    std::vector<uint8_t> wrong_data{{1, 2, 3, 4}};

    npy_array<uint8_t> arr{std::move(shape), std::move(data)};

    EXPECT_EQ(arr.byte_size(), sizeof(uint8_t) * 2 * 3);
    EXPECT_EQ(arr.dtype(), npy_dtype::uint_8());
    EXPECT_EQ(arr.fortran_order(), false);
    EXPECT_EQ(arr.size(), 2 * 3);

    EXPECT_EQ(shape.size(), 0);
    EXPECT_EQ(data.size(), 0);

    try
    {
        std::vector<size_t> shape{{2, 3}};
        std::vector<size_t> wrong_shape{{2, 3, 4}};
        std::vector<uint8_t> data{{1, 2, 3, 4, 5, 6}};
        std::vector<uint8_t> wrong_data{{1, 2, 3, 4}};

        npy_array<uint8_t>{std::move(wrong_shape), std::move(data)};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::unmatched_shape_data);
    }

    try
    {
        std::vector<size_t> shape{{2, 3}};
        std::vector<size_t> wrong_shape{{2, 3, 4}};
        std::vector<uint8_t> data{{1, 2, 3, 4, 5, 6}};
        std::vector<uint8_t> wrong_data{{1, 2, 3, 4}};
        npy_array<uint8_t>{std::move(shape), std::move(wrong_data)};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::unmatched_shape_data);
    }
    
    try
    {
        std::vector<size_t> shape{{2, 3}};
        std::vector<size_t> wrong_shape{{2, 3, 4}};
        std::vector<uint8_t> data{{1, 2, 3, 4, 5, 6}};
        std::vector<uint8_t> wrong_data{{1, 2, 3, 4}};
        npy_array<uint8_t>{std::move(wrong_shape), std::move(wrong_data)};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::unmatched_shape_data);
    }
}

TEST(NPYArrayTest, ShapeDataListConstructor)
{
    npy_array<uint8_t> arr{{2, 3}, {1, 2, 3, 4, 5, 6}};

    EXPECT_EQ(arr.byte_size(), sizeof(uint8_t) * 2 * 3);
    EXPECT_EQ(arr.dtype(), npy_dtype::uint_8());
    EXPECT_EQ(arr.fortran_order(), false);
    EXPECT_EQ(arr.size(), 2 * 3);

    try
    {
        npy_array<uint8_t>{{2}, {1, 2, 3, 4, 5, 6}};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::unmatched_shape_data);
    }

    try
    {
        npy_array<uint8_t>{{2, 3}, {1, 3, 4, 5, 6}};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::unmatched_shape_data);
    }
    
    try
    {
        npy_array<uint8_t>{{2}, {1, 2, 3, 5, 6}};
        FAIL();
    }
    catch(const npy_array_exception& e)
    {
        EXPECT_EQ(e.exception_type(), npy_array_exception_type::unmatched_shape_data);
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}