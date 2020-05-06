#ifndef D95166DE_89E6_49FF_A7EA_BA27F7948D32
#define D95166DE_89E6_49FF_A7EA_BA27F7948D32

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <regex>
#include <algorithm>
#include <numeric>
#include <functional>
#include <endian.h>

#define debug_message(message) do { if(DEBUG) std::cout << message << std::endl; } while (0)

enum endianness
{
    little_endian = '<',
    big_endian = '>'
};

endianness get_endianess()
{
    union
    {
        uint32_t integer_value;
        uint8_t byte_value[4];
    } word = {0x10203040};

    return word.byte_value[0] == 0x10 ? endianness::big_endian : endianness::little_endian;
}

enum npy_array_exception_type
{
    input_output_error,
    ill_formed_header,
    unsupported_version,
    invalid_magic_string,
};

class npy_array_exception : std::exception
{
public:
    npy_array_exception(const npy_array_exception_type exception_type) : exception_type{exception_type}{}

    const char* what()
    {
        switch (this->exception_type)
        {
        case npy_array_exception_type::input_output_error:
            return "There has been an error while opening or reading the file.";
        case npy_array_exception_type::ill_formed_header:
            return "The header of the file is ill-formed.";
        case npy_array_exception_type::unsupported_version:
            return "The version of the file is unsupported.";
        case npy_array_exception_type::invalid_magic_string:
            return "The magic string is invalid.";
        default:
            return "unknwon";
        }
    }

    npy_array_exception_type get_exception_type() const {return this->exception_type;}

private:
    npy_array_exception_type exception_type;
};

template<typename T>
class npy_array
{
public:
    npy_array(const std::string& array_path);

    npy_array(const std::vector<size_t>& shape);
    npy_array(std::vector<size_t>&& shape);

    npy_array(const std::vector<size_t>& shape, const std::vector<T>& data);
    npy_array(std::vector<size_t>&& shape, std::vector<T>&& data);

    npy_array() = delete;
    npy_array(const npy_array& other) = delete;
    npy_array(npy_array&& other) = default;

    ~npy_array() = default;

    npy_array& operator()(npy_array other) = delete;
    npy_array& operator()(const npy_array& other) = delete;
    npy_array& operator()(npy_array&& other) = delete;

    const T* get_data() const {return this->data.data();};
    T* get_data() {return this->data.data();};
    endianness get_byte_order() const {return this->byte_order;};
    size_t get_data_size() const {return this->data_size;};
    bool is_fortran_order() const {return this->fortran_order;};
    const std::vector<size_t>& get_shape() const {return this->shape;};
    size_t size() const {return std::accumulate(this->shape.begin(), this->shape.end(), 1, std::multiplies<size_t>());};
    size_t byte_size() const {return this->data_size * this->size();};

private:
    std::vector<size_t> shape;
    std::vector<T> data;
    bool fortran_order;
    endianness byte_order;
    size_t data_size;


    void parse_header(const std::string& header);
};

#include "npy_array.cpp"

#endif /* D95166DE_89E6_49FF_A7EA_BA27F7948D32 */