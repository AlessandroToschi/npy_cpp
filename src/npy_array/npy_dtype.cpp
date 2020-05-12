#include "npy_array/npy_dtype.h"

npy_dtype::npy_dtype() noexcept 
    : _kind{npy_dtype_kind::unkwown}, _item_size{0}, _byte_order{npy_endianness::not_applicable} {}

npy_dtype::npy_dtype(npy_dtype_kind kind, size_t item_size, npy_endianness byte_order) noexcept
    : _kind{kind}, _item_size{item_size}, _byte_order{byte_order} {}

npy_dtype::npy_dtype(const std::string& dtype_string) : npy_dtype()
{
    //([=<>|])?([bsuifc])(\d{1,2})?
}

npy_dtype::npy_dtype(npy_dtype&& other) 
    : _kind{std::move(other._kind)}, _item_size{std::move(other._item_size)}, _byte_order{std::move(other._byte_order)}
{
    other._kind = npy_dtype_kind::unkwown;
    other._item_size = 0;
    other._byte_order = npy_endianness::not_applicable;
}

npy_dtype& npy_dtype::operator=(npy_dtype&& other)
{
    _kind = std::move(other._kind);
    _item_size = std::move(other._item_size);
    _byte_order = std::move(other._byte_order);

    other._kind = npy_dtype_kind::unkwown;
    other._item_size = 0;
    other._byte_order = npy_endianness::not_applicable;

    return *this;
}

npy_dtype_kind npy_dtype::kind() const {return _kind;}
size_t npy_dtype::item_size() const {return _item_size;}
npy_endianness npy_dtype::byte_order() const {return _byte_order;}

npy_dtype npy_dtype::bool_8() noexcept {return npy_dtype{npy_dtype_kind::boolean, sizeof(bool)};}
npy_dtype npy_dtype::int_8() noexcept {return npy_dtype{npy_dtype_kind::integer, sizeof(int8_t)};}
npy_dtype npy_dtype::int_16() noexcept {return npy_dtype{npy_dtype_kind::integer, sizeof(int16_t)};}
npy_dtype npy_dtype::int_32() noexcept {return npy_dtype{npy_dtype_kind::integer, sizeof(int32_t)};}
npy_dtype npy_dtype::int_64() noexcept {return npy_dtype{npy_dtype_kind::integer, sizeof(int64_t)};}
npy_dtype npy_dtype::uint_8() noexcept {return npy_dtype{npy_dtype_kind::not_signed, sizeof(uint8_t)};}
npy_dtype npy_dtype::uint_16() noexcept {return npy_dtype{npy_dtype_kind::not_signed, sizeof(uint16_t)};}
npy_dtype npy_dtype::uint_32() noexcept {return npy_dtype{npy_dtype_kind::not_signed, sizeof(uint32_t)};}
npy_dtype npy_dtype::uint_64() noexcept {return npy_dtype{npy_dtype_kind::not_signed, sizeof(uint64_t)};}
npy_dtype npy_dtype::float_32() noexcept {return npy_dtype{npy_dtype_kind::floating_point, sizeof(float)};}
npy_dtype npy_dtype::float_64() noexcept {return npy_dtype{npy_dtype_kind::floating_point, sizeof(double)};}
npy_dtype npy_dtype::float_128() noexcept {return npy_dtype{npy_dtype_kind::floating_point, sizeof(long double)};}
npy_dtype npy_dtype::complex_64() noexcept {return npy_dtype{npy_dtype_kind::complex, sizeof(std::complex<float>)};}
npy_dtype npy_dtype::complex_128() noexcept {return npy_dtype{npy_dtype_kind::complex, sizeof(std::complex<double>)};}
npy_dtype npy_dtype::complex_256() noexcept {return npy_dtype{npy_dtype_kind::complex, sizeof(std::complex<long double>)};}