#include <gtest/gtest.h>
#include <typeinfo>

#include "npy_array/npy_dtype.h"

bool operator==(const npy_dtype& a, const npy_dtype& b)
{
    return a.kind() == b.kind() && a.item_size() == b.item_size() && a.byte_order() == b.byte_order();
}

bool operator!=(const npy_dtype& a, const npy_dtype& b)
{
    return !(a == b);
}

TEST(NPYDtypeTest, EmptyConstructorTest)
{
    npy_dtype empty{};

    EXPECT_EQ(empty.kind(), npy_dtype_kind::unkwown);
    EXPECT_EQ(empty.item_size(), 0);
    EXPECT_EQ(empty.byte_order(), npy_endianness::not_applicable);
}

TEST(NPYDtypeTest, CopyTest)
{
    auto dt1 = npy_dtype::bool_8();
    auto dt2{dt1};

    EXPECT_EQ(dt1, dt2);
    EXPECT_NE(&dt1, &dt2);

    auto dt3 = npy_dtype::float_128();
    auto dt4 = dt3;

    EXPECT_EQ(dt3, dt4);
    EXPECT_NE(&dt3, &dt4);
}

TEST(NPYDtypeTest, MoveTest)
{
    npy_dtype dt{};
    auto dt1 = npy_dtype::bool_8();
    auto dt2{std::move(dt1)};

    EXPECT_NE(dt1, dt2);
    EXPECT_NE(&dt1, &dt2);
    EXPECT_EQ(dt1, dt);
    EXPECT_EQ(dt2.kind(), npy_dtype_kind::boolean);
    EXPECT_EQ(dt2.item_size(), sizeof(bool));
    EXPECT_EQ(dt2.byte_order(), get_endianess());
    
    auto dt3 = npy_dtype::float_128();
    auto dt4 = std::move(dt3);

    EXPECT_NE(dt3, dt4);
    EXPECT_NE(&dt3, &dt4);
    EXPECT_EQ(dt3, dt);
    EXPECT_EQ(dt4.kind(), npy_dtype_kind::floating_point);
    EXPECT_EQ(dt4.item_size(), sizeof(long double));
    EXPECT_EQ(dt4.byte_order(), get_endianess());
}

TEST(NPYDtypeTest, BuiltinTypes)
{
    npy_endianness machine_endianess = get_endianess();
    npy_dtype dt = npy_dtype::bool_8();

    EXPECT_EQ(dt.kind(), npy_dtype_kind::boolean);
    EXPECT_EQ(dt.item_size(), sizeof(bool));
    EXPECT_EQ(dt.item_size(), 1);
    EXPECT_EQ(dt.byte_order(), npy_endianness::not_applicable);

    dt = npy_dtype::int_8();
    EXPECT_EQ(dt.kind(), npy_dtype_kind::integer);
    EXPECT_EQ(dt.item_size(), sizeof(int8_t));
    EXPECT_EQ(dt.item_size(), sizeof(signed char));
    EXPECT_EQ(dt.item_size(), sizeof(char));
    EXPECT_EQ(dt.item_size(), 1);
    EXPECT_EQ(dt.byte_order(), npy_endianness::not_applicable);

    dt = npy_dtype::int_16();
    EXPECT_EQ(dt.kind(), npy_dtype_kind::integer);
    EXPECT_EQ(dt.item_size(), sizeof(int16_t));
    EXPECT_EQ(dt.item_size(), sizeof(short));
    EXPECT_EQ(dt.item_size(), 2);
    EXPECT_EQ(dt.byte_order(), machine_endianess);

    dt = npy_dtype::int_32();
    EXPECT_EQ(dt.kind(), npy_dtype_kind::integer);
    EXPECT_EQ(dt.item_size(), sizeof(int32_t));
    EXPECT_EQ(dt.item_size(), sizeof(int));
    EXPECT_EQ(dt.item_size(), 4);
    EXPECT_EQ(dt.byte_order(), machine_endianess);

    dt = npy_dtype::int_64();
    EXPECT_EQ(dt.kind(), npy_dtype_kind::integer);
    EXPECT_EQ(dt.item_size(), sizeof(int64_t));
    EXPECT_EQ(dt.item_size(), sizeof(long));
    EXPECT_EQ(dt.item_size(), 8);
    EXPECT_EQ(dt.byte_order(), machine_endianess);

    dt = npy_dtype::uint_8();
    EXPECT_EQ(dt.kind(), npy_dtype_kind::not_signed);
    EXPECT_EQ(dt.item_size(), sizeof(uint8_t));
    EXPECT_EQ(dt.item_size(), sizeof(unsigned char));
    EXPECT_EQ(dt.item_size(), 1);
    EXPECT_EQ(dt.byte_order(), npy_endianness::not_applicable);

    dt = npy_dtype::uint_16();
    EXPECT_EQ(dt.kind(), npy_dtype_kind::not_signed);
    EXPECT_EQ(dt.item_size(), sizeof(uint16_t));
    EXPECT_EQ(dt.item_size(), sizeof(unsigned short));
    EXPECT_EQ(dt.item_size(), 2);
    EXPECT_EQ(dt.byte_order(), machine_endianess);

    dt = npy_dtype::uint_32();
    EXPECT_EQ(dt.kind(), npy_dtype_kind::not_signed);
    EXPECT_EQ(dt.item_size(), sizeof(uint32_t));
    EXPECT_EQ(dt.item_size(), sizeof(unsigned int));
    EXPECT_EQ(dt.item_size(), 4);
    EXPECT_EQ(dt.byte_order(), machine_endianess);

    dt = npy_dtype::uint_64();
    EXPECT_EQ(dt.kind(), npy_dtype_kind::not_signed);
    EXPECT_EQ(dt.item_size(), sizeof(uint64_t));
    EXPECT_EQ(dt.item_size(), sizeof(unsigned long));
    EXPECT_EQ(dt.item_size(), 8);
    EXPECT_EQ(dt.byte_order(), machine_endianess);

    dt = npy_dtype::float_32();
    EXPECT_EQ(dt.kind(), npy_dtype_kind::floating_point);
    EXPECT_EQ(dt.item_size(), sizeof(float));
    EXPECT_EQ(dt.item_size(), 4);
    EXPECT_EQ(dt.byte_order(), machine_endianess);

    dt = npy_dtype::float_64();
    EXPECT_EQ(dt.kind(), npy_dtype_kind::floating_point);
    EXPECT_EQ(dt.item_size(), sizeof(double));
    EXPECT_EQ(dt.item_size(), 8);
    EXPECT_EQ(dt.byte_order(), machine_endianess);

    dt = npy_dtype::float_128();
    EXPECT_EQ(dt.kind(), npy_dtype_kind::floating_point);
    EXPECT_EQ(dt.item_size(), sizeof(long double));
    EXPECT_EQ(dt.item_size(), 16);
    EXPECT_EQ(dt.byte_order(), machine_endianess);

    dt = npy_dtype::complex_64();
    EXPECT_EQ(dt.kind(), npy_dtype_kind::complex);
    EXPECT_EQ(dt.item_size(), sizeof(std::complex<float>));
    EXPECT_EQ(dt.item_size(), 8);
    EXPECT_EQ(dt.byte_order(), machine_endianess);

    dt = npy_dtype::complex_128();
    EXPECT_EQ(dt.kind(), npy_dtype_kind::complex);
    EXPECT_EQ(dt.item_size(), sizeof(std::complex<double>));
    EXPECT_EQ(dt.item_size(), 16);
    EXPECT_EQ(dt.byte_order(), machine_endianess);

    dt = npy_dtype::complex_256();
    EXPECT_EQ(dt.kind(), npy_dtype_kind::complex);
    EXPECT_EQ(dt.item_size(), sizeof(std::complex<long double>));
    EXPECT_EQ(dt.item_size(), 32);
    EXPECT_EQ(dt.byte_order(), machine_endianess);
}

TEST(NPYDtypeTest, FromType)
{
    auto dt = npy_dtype::from_type<bool>();
    auto test = npy_dtype::bool_8();
    EXPECT_EQ(dt, test);

    dt = npy_dtype::from_type<char>();
    test = npy_dtype::int_8();
    EXPECT_EQ(dt, test);
    dt = npy_dtype::from_type<signed char>();
    EXPECT_EQ(dt, test);

    dt = npy_dtype::from_type<short>();
    test = npy_dtype::int_16();
    EXPECT_EQ(dt, test);

    dt = npy_dtype::from_type<int>();
    test = npy_dtype::int_32();
    EXPECT_EQ(dt, test);

    dt = npy_dtype::from_type<long>();
    test = npy_dtype::int_64();
    EXPECT_EQ(dt, test);

    dt = npy_dtype::from_type<unsigned char>();
    test = npy_dtype::uint_8();
    EXPECT_EQ(dt, test);

    dt = npy_dtype::from_type<unsigned short>();
    test = npy_dtype::uint_16();
    EXPECT_EQ(dt, test);

    dt = npy_dtype::from_type<unsigned int>();
    test = npy_dtype::uint_32();
    EXPECT_EQ(dt, test);

    dt = npy_dtype::from_type<unsigned long>();
    test = npy_dtype::uint_64();
    EXPECT_EQ(dt, test);

    dt = npy_dtype::from_type<float>();
    test = npy_dtype::float_32();
    EXPECT_EQ(dt, test);

    dt = npy_dtype::from_type<double>();
    test = npy_dtype::float_64();
    EXPECT_EQ(dt, test);

    dt = npy_dtype::from_type<long double>();
    test = npy_dtype::float_128();
    EXPECT_EQ(dt, test);

    dt = npy_dtype::from_type<std::complex<float>>();
    test = npy_dtype::complex_64();
    EXPECT_EQ(dt, test);

    dt = npy_dtype::from_type<std::complex<double>>();
    test = npy_dtype::complex_128();
    EXPECT_EQ(dt, test);

    dt = npy_dtype::from_type<std::complex<long double>>();
    test = npy_dtype::complex_256();
    EXPECT_EQ(dt, test);
}

TEST(NPYDtypeTest, FromString)
{
    npy_dtype::from_string("f");
    npy_dtype p = npy_dtype::from_string("b1");
    npy_dtype g = npy_dtype::from_string("<b");
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}