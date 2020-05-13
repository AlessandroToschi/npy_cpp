#ifndef D4AB99B8_10DA_46D7_9CFA_559B460F6ABA
#define D4AB99B8_10DA_46D7_9CFA_559B460F6ABA

#include <cstddef>
#include <type_traits>
#include <complex>

#include <boost/regex.hpp>

#include "npy_array/endianess.h"
#include "npy_array/npy_exception.h"

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
    npy_dtype(npy_dtype&& other);

    ~npy_dtype() = default;
    
    npy_dtype& operator=(const npy_dtype& other) = default;
    npy_dtype& operator=(npy_dtype&& other);

    npy_dtype_kind kind() const;
    size_t item_size() const;
    npy_endianness byte_order() const;

    template<typename T> static npy_dtype from_type() noexcept
    {
       if(std::is_same<T, bool>::value) return npy_dtype::bool_8();
       else if(std::is_same<T, int8_t>::value || std::is_same<T, char>::value) return npy_dtype::int_8();
       else if(std::is_same<T, int16_t>::value) return npy_dtype::int_16();
       else if(std::is_same<T, int32_t>::value) return npy_dtype::int_32();
       else if(std::is_same<T, int64_t>::value) return npy_dtype::int_64();
       else if(std::is_same<T, uint8_t>::value) return npy_dtype::uint_8();
       else if(std::is_same<T, uint16_t>::value) return npy_dtype::uint_16();
       else if(std::is_same<T, uint32_t>::value) return npy_dtype::uint_32();
       else if(std::is_same<T, uint64_t>::value) return npy_dtype::uint_64();
       else if(std::is_same<T, float>::value) return npy_dtype::float_32();
       else if(std::is_same<T, double>::value) return npy_dtype::float_64();
       else if(std::is_same<T, long double>::value) return npy_dtype::float_128();
       else if(std::is_same<T, std::complex<float>>::value) return npy_dtype::complex_64();
       else if(std::is_same<T, std::complex<double>>::value) return npy_dtype::complex_128();
       else if(std::is_same<T, std::complex<long double>>::value) return npy_dtype::complex_256();
       else return npy_dtype::null();
    }
    static npy_dtype from_string(const std::string& dtype_string) noexcept;

    static npy_dtype bool_8() noexcept;
    static npy_dtype int_8() noexcept;
    static npy_dtype int_16() noexcept;
    static npy_dtype int_32() noexcept;
    static npy_dtype int_64() noexcept;
    static npy_dtype uint_8() noexcept;
    static npy_dtype uint_16() noexcept;
    static npy_dtype uint_32() noexcept;
    static npy_dtype uint_64() noexcept;
    static npy_dtype float_32() noexcept;
    static npy_dtype float_64() noexcept;
    static npy_dtype float_128() noexcept;
    static npy_dtype complex_64() noexcept;
    static npy_dtype complex_128() noexcept;
    static npy_dtype complex_256() noexcept;    
    static npy_dtype null() noexcept;
    
private:
    npy_dtype(npy_dtype_kind kind, size_t item_size, npy_endianness byte_order=get_endianess()) noexcept;

    npy_dtype_kind _kind;
    size_t _item_size;
    npy_endianness _byte_order;
};

#endif /* D4AB99B8_10DA_46D7_9CFA_559B460F6ABA */
