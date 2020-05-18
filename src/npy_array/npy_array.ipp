#include "npy_array/npy_array.h"

template<typename T>
npy_array<T>::npy_array(const std::string& array_path)
{
    // A NumPy array file always starts with the following magic string '\x93NUMPY'.
    std::string magic_string; 
    // The major version of the NumPy array. There exists three version:
    // Version 1: the classic NumPy array having an header of maximum size of 65536 bytes (16-bit).
    // Version 2: the same as the version 1, but it allows an header of maximum size of 2^32 (32-bit).
    // Version 3: the same as the version 1, butthe field names are encoded in UTF-8 instead of ASCII.
    uint8_t major_version; 
    uint8_t minor_version; // The minor version of the file, just for reference.
    // The header length in bytes.
    // Version 1 header length is 2 bytes.
    uint16_t header_length;
    // The header of the file is a Python dictionary represented as string which contains three fields:
    // 'desc': the NumPy.dtype object representation.
    // 'fortran_order': true if the values in the array are stored in fortran order, otherwise false.
    // 'shape': the shape of the array.
    // The header is terminated by a '\n' and padded with spaces ('\x20') to make the whole header even divisible by 64.
    std::string header;
    // The input stream object that is used to read the file.
    std::ifstream array_stream;

    // Get the endianess of this system.
    npy_endianness machine_endianess = get_endianess();

    // Convert all the iostate bits to exceptions.
    array_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);

    try
    {
        // Open the file.
        array_stream.open(array_path);

        // The magic string is exactly 6 bytes, so we reserve that size to be able to directly read those 6 bytes in the string.
        magic_string.resize(6);
        array_stream.read(&magic_string[0], 6);

        // Check if the magic string is corrected, otherwise throw an exception.
        // The magic string is hard-coded and endianess-invariant since it is a string.
        if(magic_string != "\x93NUMPY")
        {
            throw npy_array_exception{npy_array_exception_type::invalid_magic_string};
        }

        // Read the major and minor version.
        array_stream.read(reinterpret_cast<char*>(&major_version), 1);
        array_stream.read(reinterpret_cast<char*>(&minor_version), 1);

        // If the version is 1, then the header length is 2 bytes.
        // We use a temporary variable of 2 bytes and then copy the result to the other header_length.
        // The header length is stored in little-endian order so if we read it in a little endian system there is no problem, otherwise we must reverse the bytes.
        // To reverse the bytes, we use the intrinsic defined in endian.h
        if(major_version == 0x01)
        {
            array_stream.read(reinterpret_cast<char*>(&header_length), 2);
            header_length = machine_endianess == npy_endianness::little_endian ? header_length : __bswap_16(header_length);
        }
        else
        {
            throw npy_array_exception{npy_array_exception_type::unsupported_version};
        }
        /*
        // If the version is 2, then the header length is 4 bytes.
        // It is directly read in the header_length of the function.
        // The same consideration for the endianess applies for the version 2 header length.
        // Still need to working on, so I consider the version 2 as unsupported.
        else if(major_version == 0x02)
        {
            array_stream.read(reinterpret_cast<char*>(&header_length), 4);

            if(machine_endianess == npy_endianness::big_endian)
            {
                header_length = __bswap_32(header_length);
            }

            throw npy_array_exception{npy_array_exception_type::unsupported_version};
        }
        // Version 3 still not supported or invalid version found, throw an exception.

        */
        
        // Reserve the required memory for storing the header and read it from the file.
        // If the memory reservation fails (very unlikely tho), it will throw a bad_alloc exception, which is caught.
        header.resize(header_length);
        array_stream.read(&header[0], header_length);

        // Parse the header of the file.
        // This function will assign a value to the following field: shape, fortran_order, byte_order, and item size.
        // If the header parse fails to assign a value to any of such fields, then this function will throw an exception.
        this->parse_header(header);

        //this->data.resize(this->size());

        //array_stream.read(reinterpret_cast<char*>(this->data.data()), this->byte_size());
    }
    catch(const std::ios_base::failure& failure_exception)
    {
        throw npy_array_exception{npy_array_exception_type::input_output_error};
    }
    catch(const boost::regex_error& regex_exception)
    {
        throw npy_array_exception{npy_array_exception_type::ill_formed_header};
    }
    catch(const std::bad_alloc& bad_alloc_exception)
    {
        throw npy_array_exception{npy_array_exception_type::unsufficient_memory};
    }
    catch(const std::exception& exeption)
    {
        throw npy_array_exception{npy_array_exception_type::generic};
    }
}

template<typename T>
npy_array<T>::npy_array(const std::vector<size_t>& shape)
    : _shape{shape}, _data{}, _dtype{}, _fortran_order{false}
{
    npy_dtype requested_dtype = npy_dtype::from_type<T>();

    if(requested_dtype)
    {
        _dtype = std::move(requested_dtype);
    }
    else
    {
        //THROW SOMETHING
    }
    
    _data.resize(this->size());
}

template<typename T>
npy_array<T>::npy_array(std::vector<size_t>&& shape)
    : _shape{std::move(shape)}, _data{}, _dtype{}, _fortran_order{false}
{
    npy_dtype requested_dtype = npy_dtype::from_type<T>();

    if(requested_dtype)
    {
        _dtype = std::move(requested_dtype);
    }
    else
    {
        //THROW SOMETHING
    }
    
    _data.resize(this->size());
}

template<typename T>
npy_array<T>::npy_array(const std::vector<size_t>& shape, const std::vector<T>& data)
    : _shape{}, _data{}, _dtype{}, _fortran_order{false}
{
    if(std::accumulate(shape.cbegin(), shape.cend(), 1, std::multiplies<size_t>()) == data.size())
    {
        npy_dtype requested_dtype = npy_dtype::from_type<T>();

        if(requested_dtype)
        {
            _dtype = std::move(requested_dtype);
        }
        else
        {
            /* code */
        }
        

        _shape = shape;
        _data = data;
    }
    else
    {
        /* code */
    }
    
} 

template<typename T>
npy_array<T>::npy_array(std::vector<size_t>&& shape, std::vector<T>&& data)
    : _shape{}, _data{}, _dtype{}, _fortran_order{false}
{
    if(std::accumulate(shape.cbegin(), shape.cend(), 1, std::multiplies<size_t>()) == data.size())
    {
        npy_dtype requested_dtype = npy_dtype::from_type<T>();

        if(requested_dtype)
        {
            _dtype = std::move(requested_dtype);
        }
        else
        {
            /* code */
        }
        

        _shape = std::move(shape);
        _data = std::move(data);
    }
    else
    {
        /* code */
    }
    
}

template<typename T>
size_t npy_array<T>::size() const noexcept
{
    return std::accumulate(_shape.cbegin(), _shape.cend(), 1, std::multiplies<size_t>());
}

template<typename T>
void npy_array<T>::parse_header(const std::string& header)
{
    std::array<char, 5> charaters_to_remove{' ', '\n', '{', '}', '\''};
    // Create a copy of the header string because we need to modify it.
    auto header_copy{header};
    // Remove the unwanted characters from the header copy string.
    header_copy.erase(std::remove_if(header_copy.begin(), header_copy.end(), [&](char c)
    {
        return std::find(charaters_to_remove.cbegin(), charaters_to_remove.cend(), c) != charaters_to_remove.cend();
    }), header_copy.end());

    // Find the indexes of the left and right brackes within the header string.
    // Both key value pairs and shape elements are comma-separeted.
    // We need to change the separation of the shape by replacing the comma in the shape string with another token that easily recognizable, like an underscore.
    auto left_bracket_index = std::find(header_copy.begin(), header_copy.end(), '(');
    auto right_bracket_index = std::find(header_copy.begin(), header_copy.end(), ')');

    if(left_bracket_index != header_copy.end() && right_bracket_index != header_copy.end())
    {
        std::replace(left_bracket_index, right_bracket_index, ',', '_');
    }
    else
    {
        throw boost::regex_error{boost::regex_constants::error_unknown};
    }
    

    std::vector<std::string> header_key_value_pairs;

    boost::split(header_key_value_pairs, header_copy, [](char c){return c == ',';});

    for(auto& key_value_pair : header_key_value_pairs)
    {
        std::vector<std::string> key_value{};
        boost::split(key_value, key_value_pair, [](char c){return c == ':';});

        if(key_value[0] == "descr")
        {
            npy_dtype requested_dtype = npy_dtype::from_string(key_value[1]);

            if(requested_dtype && npy_dtype::from_type<T>() == requested_dtype)
            {
                _dtype = std::move(requested_dtype);
            }
            else
            {
                throw boost::regex_error{boost::regex_constants::error_unknown};
            }
            
        }
        else if(key_value[0] == "fortran_order")
        {
            _fortran_order = key_value[1] == "False" ? false : true;
        }
        else if(key_value[0] == "shape")
        {
            std::vector<std::string> shape_values{};
            boost::split(shape_values, key_value[1].substr(1, key_value[1].size() - 2), [](char c){return c == '_';});
            _shape.reserve(shape_values.size());
            std::transform(shape_values.cbegin(), shape_values.cend(), std::back_inserter(_shape), [](std::string shape_value) -> size_t
            {
                return std::strtoul(shape_value.data(), nullptr, 10);
            });
        }
        else
        {
            throw boost::regex_error{boost::regex_constants::error_unknown};
        }
        
    }
}