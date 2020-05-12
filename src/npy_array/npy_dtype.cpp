#include "npy_array/npy_dtype.h"

npy_dtype::npy_dtype() noexcept 
    : _kind{npy_dtype_kind::unkwown}, _item_size{0}, _byte_order{get_endianess()} {}

npy_dtype::npy_dtype(npy_dtype_kind kind, size_t item_size, npy_endianness byte_order) noexcept
    : _kind{kind}, _item_size{item_size}, _byte_order{byte_order} {}

npy_dtype_kind npy_dtype::kind() const {return _kind;}
size_t npy_dtype::item_size() const {return _item_size;}
npy_endianness npy_dtype::byte_order() const {return _byte_order;}

template<typename T> 
static npy_dtype from_type()
{
    return npy_dtype{};
}

static npy_dtype binary() {return npy_dtype{npy_dtype_kind::boolean, sizeof(bool)};}
static npy_dtype int_8();
static npy_dtype int_16();
static npy_dtype int_32();
static npy_dtype int_64();
static npy_dtype uint_8();
static npy_dtype uint_16();
static npy_dtype uint_32();
static npy_dtype uint_64();
static npy_dtype float_16();
static npy_dtype float_32();
static npy_dtype float_64();
static npy_dtype float_128();
static npy_dtype complex_64();
static npy_dtype complex_128();
static npy_dtype complex_256();