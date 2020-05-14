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

std::string npy_dtype::str() const
{
    std::string dtype_string{};
    dtype_string.push_back(static_cast<char>(_byte_order));
    dtype_string.push_back(static_cast<char>(_kind));
    dtype_string.append(std::to_string(_item_size));
    return dtype_string;
}

npy_dtype npy_dtype::from_string(const std::string& dtype_string) noexcept
{
    boost::regex dtype_pattern{R"((\|b1|\|u1|\|i1)|(^[<>=]?)((?<!\|)[iufc])((?<=[iu])2|(?<=[iuf])4|(?<=[uifc])8|(?<=[fc])16|(?<=[c])32)?$)"};
    boost::match_results<std::string::const_iterator> match_results{};

    npy_dtype_kind kind;
    size_t item_size;
    npy_endianness byte_order = get_endianess();

    if(boost::regex_search(dtype_string.cbegin(), dtype_string.cend(), match_results, dtype_pattern))
    {
        if(match_results.size() == 5)
        {
            if(match_results[1].matched)
            {
                auto one_byte_dtype = match_results[1].str();

                if(one_byte_dtype == "|b1") return npy_dtype::bool_8();
                else if(one_byte_dtype == "|i1") return npy_dtype::int_8();
                else if(one_byte_dtype == "|u1") return npy_dtype::uint_8();
                else return npy_dtype::null();
            }

            if(match_results[2].matched)
            {
                npy_endianness requested_endianess = static_cast<npy_endianness>(match_results[2].str()[0]);

                if(requested_endianess != npy_endianness::native) byte_order = requested_endianess;
            }

            if(match_results[3].matched) kind = static_cast<npy_dtype_kind>(match_results[3].str()[0]);
            else return npy_dtype::null();

            if(match_results[4].matched) item_size = std::stoul(match_results[4].str());
            else
            {
                switch (kind)
                {
                case npy_dtype_kind::integer:
                case npy_dtype_kind::floating_point:
                case npy_dtype_kind::not_signed:
                    item_size = 8;
                    break;
                case npy_dtype_kind::complex:
                    item_size = 16;
                    break;
                default:
                    return npy_dtype::null();
                }
            }
            return npy_dtype(kind, item_size, byte_order);
        }
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