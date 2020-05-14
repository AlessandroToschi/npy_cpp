#ifndef D4AB99B8_10DA_46D7_9CFA_559B460F6ABA
#define D4AB99B8_10DA_46D7_9CFA_559B460F6ABA

#include <cstddef>
#include <type_traits>
#include <complex>

#include <boost/regex.hpp>

#include "npy_array/endianess.h"
#include "npy_array/npy_exception.h"

/**
 * @brief Enumerator that describes the possible types of a dtype.
 * 
 * Only native dtytes are supported: boolean, integers, unsigned integers, floating points, and complex.
 * The structured dtypes are not supported.
 * Each kind is backed by a char that uniquely identifies the kind and it is part of the dtype string format.
 * The 'unkown' kind is an artificial kind used to express a not valid kind, it is used by the null dtype.
 * 
 */
enum npy_dtype_kind
{
    boolean = 'b',
    integer = 'i',
    not_signed = 'u', // unsigned integers.
    floating_point = 'f',
    complex = 'c',
    unkwown = '!' // artificial kind used for the null dtype.
};

/**
 * @brief Class that expresses a NumPy dtype object.
 * 
 * A dtype object is identified by three attributes:
 * kind: the kind of the type.
 * item size: the size in bytes of the dtype object.
 * byte order: the endianess of the dtype object.
 * 
 * The byte order applies only for an dtype object with item size >= 2.
 * For an dtype object with item size 1, the byte order is not applicable.
 * 
 * A npy_dtype object is immutable and read-only.
 * The use can create a npy_dtype using two static methods:
 * 
 * npy_dtype::from_type<T>(), which creates the npy_dtype given the C++ type.
 * If the C++ type is not one of the implemented, then it is returned the null dtype.
 * 
 * npy_dtype::from_string(dtype_string), which creates the npy_dtype given a NumPy dtype object string format.
 * If the dtype_string is not one of the implemented or malformed, then it is returned the null dtype.
 * 
 * Alternatively, the user can select one of the native types provided as static methods.
 * 
 * The implemented types are:
 * bool_8, 8-bit boolean value backed by C++ bool type.
 * int_8, signed 8-bit integer value backed by C++ int8_t and char.
 * int_16, signed 16-bit integer value backed by C++ int16_t and short.
 * int_32, signed 32-bit integer value backed by C++ int32_t and int.
 * int_64, signed 64-bit integer value backed by C++ int64_t and long.
 * uint_8, unsigned 8-bit integer value backed by C++ uint8_t and unsigned char.
 * uint_16, unsigned 16-bit integer value backed by C++ uint16_t and unsigned short.
 * uint_32, unsigned 32-bit integer value backed by C++ uint32_t and unsigned int.
 * uint_64, unsigned 64-bit integer value backed by C++ uint64_t and unsigned long.
 * float_32, 32-bit floating point value backed by C++ float.
 * float_64, 64-bit floating point value backed by C++ double.
 * float_128, 128-bit floating point value backed by C++ long double.
 * complex_64, 64-bit complex value backed by C++ std::complex<float>.
 * complex_128, 128-bit complex value backed by C++ std::complex<double>.
 * complex_256, 256-bit complex value backed by C++ std::complex<long double>.
 * 
 * The whole object does not throw exceptions, but when errors occur (during the creation of an object) the null dtype is returned.
 * If the user builds its own dtype, either by providing a type or a dtype string, it must check if the retrieved dtype is not equal to the null dtype.
 * A null dtype has the following invariants:
 * kind = npy_dtype_kind::unknown
 * item size = 0
 * byte order = npy_endianess::not_applicable.
 */
class npy_dtype
{
public:
    /**
     * @brief Construct a null npy_dtype object.
     * 
     * The null dtype object has the following invariants:
     * kind = npy_dtype_kind::unknown
     * item size = 0
     * byte order = npy_endianess::not_applicable.
     */
    npy_dtype() noexcept;

    /**
     * @brief Copy constructor, construct a dtype object given another one.
     * 
     * The copy constructor is compiler-generated.
     * 
     * @param other another dtype object.
     */
    npy_dtype(const npy_dtype& other) = default;
    /**
     * @brief Move constructor, construct a new dtype object by moving another one.
     * 
     * The move constructor leaves the other object identical to the null dtype.
     * 
     * @param other another dtype object.
     */
    npy_dtype(npy_dtype&& other);

    /**
     * @brief Destroy the npy_dtype object
     * 
     * Compiler-generated, this class contains only builtin types and no dynamic memory is involed.
     * 
     */
    ~npy_dtype() = default;
    
    /**
     * @brief Copy assignment, copy the given dtype object to this.
     * 
     * The copy assignment is compiler-generated.
     * 
     * @param other the dtype object to copy.
     * @return npy_dtype& this object.
     */
    npy_dtype& operator=(const npy_dtype& other) = default;
    npy_dtype& operator=(npy_dtype&& other);

    npy_dtype_kind kind() const;
    size_t item_size() const;
    npy_endianness byte_order() const;
    std::string str() const;

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
