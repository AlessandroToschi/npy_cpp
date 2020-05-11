#ifndef D95166DE_89E6_49FF_A7EA_BA27F7948D32
#define D95166DE_89E6_49FF_A7EA_BA27F7948D32

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <boost/regex.hpp>
#include <algorithm>
#include <numeric>
#include <functional>
#include <endian.h>

#include "endianess.h"
#include "npy_exception.h"

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

    npy_endianness get_byte_order() const {return this->byte_order;};

    size_t get_item_size() const {return this->item_size;};

    bool is_fortran_order() const {return this->fortran_order;};

    const std::vector<size_t>& get_shape() const {return this->shape;};

    size_t size() const {return std::accumulate(this->shape.begin(), this->shape.end(), 1, std::multiplies<size_t>());};
    
    size_t byte_size() const {return this->item_size * this->size();};

private:
    std::vector<size_t> shape;
    std::vector<T> data;
    bool fortran_order;
    npy_endianness byte_order;
    size_t item_size;


    void parse_header(const std::string& header);
};

#include "npy_array.ipp"

#endif /* D95166DE_89E6_49FF_A7EA_BA27F7948D32 */