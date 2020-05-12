#ifndef D4AB99B8_10DA_46D7_9CFA_559B460F6ABA
#define D4AB99B8_10DA_46D7_9CFA_559B460F6ABA

#include <cstddef>
#include <type_traits>

#include "npy_array/endianess.h"

enum npy_dtype_kind
{
    boolean = 'b',
    integer = 'i',
    not_signed = 'u',
    floating_point = 'f',
    complex = 'c',
    unkwown = '\0'
};

class npy_dtype
{
public:
    npy_dtype() noexcept;

    npy_dtype(const npy_dtype& other) = default;
    npy_dtype(npy_dtype&& other) = default;

    ~npy_dtype() = default;
    
    npy_dtype& operator=(const npy_dtype& other) = default;
    npy_dtype& operator=(npy_dtype&& other) = default;

    npy_dtype_kind kind() const;
    size_t item_size() const;
    npy_endianness byte_order() const;

    template<typename T> static npy_dtype from_type() noexcept;
    
    static npy_dtype binary();
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
private:
    npy_dtype(npy_dtype_kind kind, size_t item_size, npy_endianness byte_order=get_endianess()) noexcept;

    npy_dtype_kind _kind;
    size_t _item_size;
    npy_endianness _byte_order;
};

#endif /* D4AB99B8_10DA_46D7_9CFA_559B460F6ABA */
