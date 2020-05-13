#include "npy_array/npy_dtype.h"

#include <iostream>

npy_dtype::npy_dtype() noexcept 
    : _kind{npy_dtype_kind::unkwown}, _item_size{0}, _byte_order{npy_endianness::not_applicable} {}

npy_dtype::npy_dtype(npy_dtype_kind kind, size_t item_size, npy_endianness byte_order) noexcept
    : _kind{kind}, _item_size{item_size}, _byte_order{byte_order} {}

npy_dtype::npy_dtype(npy_dtype&& other) 
    : _kind{std::move(other._kind)}, _item_size{std::move(other._item_size)}, _byte_order{std::move(other._byte_order)}
{
    if(this != &other)
    {
        other._kind = npy_dtype_kind::unkwown;
        other._item_size = 0;
        other._byte_order = npy_endianness::not_applicable;
    }
}

npy_dtype& npy_dtype::operator=(npy_dtype&& other)
{
    if(this != &other)
    {
        _kind = std::move(other._kind);
        _item_size = std::move(other._item_size);
        _byte_order = std::move(other._byte_order);

        other._kind = npy_dtype_kind::unkwown;
        other._item_size = 0;
        other._byte_order = npy_endianness::not_applicable;
    }

    return *this;
}

npy_dtype_kind npy_dtype::kind() const {return _kind;}
size_t npy_dtype::item_size() const {return _item_size;}
npy_endianness npy_dtype::byte_order() const {return _byte_order;}

npy_dtype npy_dtype::from_string(const std::string& dtype_string) noexcept
{
    //([=<>|])?([bsuifc])(\d{1,2})?
    boost::regex dtype_pattern{R"(([=<>|])?((b1?)|(i|u)(1|2|4|8)?|((f)(4|8|16)?)|(c)(8|16|32)?)$)"};
    boost::match_results<std::string::const_iterator> match_results{};

    npy_dtype_kind kind;
    size_t item_size;
    npy_endianness byte_order = get_endianess();

    if(boost::regex_search(dtype_string.cbegin(), dtype_string.cend(), match_results, dtype_pattern))
    {
        if(match_results.size() == 11)
        {
            if(match_results[1].str() != "")
            {
                npy_endianness requested_byte_order = static_cast<npy_endianness>(match_results[1].str()[0]);
                byte_order = requested_byte_order == npy_endianness::native ? get_endianess() : requested_byte_order;
            }
        }
        /*
        if(match_results.size() == 4)
        {
            auto byte_o = match_results[1].str();
        }
        if(match_results.size() == 2)
        {
            switch (static_cast<npy_dtype_kind>(match_results[1].str()[0]))
            {
            case npy_dtype_kind::boolean:
                return npy_dtype::bool_8();
            case npy_dtype_kind::integer:
                return npy_dtype::int_64();
            case npy_dtype_kind::not_signed:
                return npy_dtype::uint_64();
            case npy_dtype_kind::floating_point:
                return npy_dtype::float_64();
            case npy_dtype_kind::complex:
                return npy_dtype::complex_128();
            default:
                return npy_dtype::null();
            }
        }
        else if(match_results.size() == 3)
        {
            
        }
        for(auto& x : match_results)
        {
            std::cout << x.str() << std::endl;
        }
        */
    }
    return npy_dtype::null();
}

npy_dtype npy_dtype::bool_8() noexcept {return npy_dtype{npy_dtype_kind::boolean, sizeof(bool), npy_endianness::not_applicable};}
npy_dtype npy_dtype::int_8() noexcept {return npy_dtype{npy_dtype_kind::integer, sizeof(int8_t),npy_endianness::not_applicable};}
npy_dtype npy_dtype::int_16() noexcept {return npy_dtype{npy_dtype_kind::integer, sizeof(int16_t)};}
npy_dtype npy_dtype::int_32() noexcept {return npy_dtype{npy_dtype_kind::integer, sizeof(int32_t)};}
npy_dtype npy_dtype::int_64() noexcept {return npy_dtype{npy_dtype_kind::integer, sizeof(int64_t)};}
npy_dtype npy_dtype::uint_8() noexcept {return npy_dtype{npy_dtype_kind::not_signed, sizeof(uint8_t), npy_endianness::not_applicable};}
npy_dtype npy_dtype::uint_16() noexcept {return npy_dtype{npy_dtype_kind::not_signed, sizeof(uint16_t)};}
npy_dtype npy_dtype::uint_32() noexcept {return npy_dtype{npy_dtype_kind::not_signed, sizeof(uint32_t)};}
npy_dtype npy_dtype::uint_64() noexcept {return npy_dtype{npy_dtype_kind::not_signed, sizeof(uint64_t)};}
npy_dtype npy_dtype::float_32() noexcept {return npy_dtype{npy_dtype_kind::floating_point, sizeof(float)};}
npy_dtype npy_dtype::float_64() noexcept {return npy_dtype{npy_dtype_kind::floating_point, sizeof(double)};}
npy_dtype npy_dtype::float_128() noexcept {return npy_dtype{npy_dtype_kind::floating_point, sizeof(long double)};}
npy_dtype npy_dtype::complex_64() noexcept {return npy_dtype{npy_dtype_kind::complex, sizeof(std::complex<float>)};}
npy_dtype npy_dtype::complex_128() noexcept {return npy_dtype{npy_dtype_kind::complex, sizeof(std::complex<double>)};}
npy_dtype npy_dtype::complex_256() noexcept {return npy_dtype{npy_dtype_kind::complex, sizeof(std::complex<long double>)};}
npy_dtype npy_dtype::null() noexcept {static npy_dtype null_dtype{}; return null_dtype;}