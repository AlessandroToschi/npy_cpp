#include "npy_array.h"

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
    uint16_t header_length_v1; // The header length in bytes for the version 1 file format.
    uint32_t header_length_v2; // The header length in bytes for the version 2 file format.
    // The header of the file is a Python dictionary represented as string which contains three fields:
    // 'desc': the NumPy.dtype object representation.
    // 'fortran_order': true if the values in the array are stored in fortran order, otherwise false.
    // 'shape': the shape of the array.
    // The header is terminated by a '\n' and padded with spaces ('\x20') to make the whole header even divisible by 64.
    std::string header;
    // The input stream object that is used to read the file.
    std::ifstream array_stream;

    // Convert all the iostate bits to exceptions.
    array_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);

    debug_message("NPY Array: opening file path " << array_path);

    try
    {
        // Open the file.
        array_stream.open(array_path);

        debug_message("NPY Array: file opened successfully at path " << array_path);

        // The magic string is exactly 6 bytes, so we reserve that size to be able to directly read those 6 bytes in the string.
        magic_string.resize(6);
        array_stream.read(&magic_string[0], 6);

        debug_message("NPY Array: magic string is " << magic_string);


        if(magic_string != "\x93NUMPY")
        {
            debug_message("NPY Array: magic string is incorrect");
            throw npy_array_exception{npy_array_exception_type::invalid_magic_string};
        }

        array_stream.read(reinterpret_cast<char*>(&major_version), 1);
        array_stream.read(reinterpret_cast<char*>(&minor_version), 1);

        debug_message("NPY Array: version " << static_cast<int>(major_version) << "." << static_cast<int>(minor_version));

        if(major_version != 0x01)
        {
            debug_message("NPY Array: unsupported major version " << static_cast<int>(major_version));
            throw npy_array_exception{npy_array_exception_type::unsupported_version};
        }

        array_stream.read(reinterpret_cast<char*>(&header_length_v1), 2);

        debug_message("NPY Array: header length " << header_length << " bytes");

        header.resize(header_length);
        array_stream.read(&header[0], header_length);

        debug_message("NPY Array: header '" << header << "'");

        this->parse_header(header);
    }
    catch(const std::ios_base::failure& failure_exception)
    {
        throw npy_array_exception{npy_array_exception_type::input_output_error};
    }
    catch(const std::regex_error& regex_exception)
    {
        throw npy_array_exception{npy_array_exception_type::ill_formed_header};
    }
}

template<typename T>
void npy_array<T>::parse_header(const std::string& header)
{
    auto header_copy{header};
    header_copy.erase(std::remove_if(header_copy.begin(), header_copy.end(), [](char c){
        return std::isspace(c) || c == '\n';
    }), header_copy.end());

    debug_message("NPY Array: striped header '" << header_copy << "'");

    std::regex pattern{R"('(\w+)':('([<>]\w+)'|\w+|\(\d+(?:,\d+)*\)))"};
    std::sregex_iterator next_iterator{header_copy.begin(), header_copy.end(), pattern};
    std::sregex_iterator end_iterator{};
    
    while(next_iterator != end_iterator)
    {
        const auto match = *next_iterator;

        if(match.size() != 4)
        {
            throw std::regex_error{std::regex_constants::error_type::_S_error_backref};
        }

        const auto key = match[1].str();

        if(key == "descr")
        {
            const auto description = match[3].str();

            if(description.size() != 3)
            {

            }
            
            auto x = get_endianess();
        }
        else if(key == "fortran_order")
        {

        }
        else if(key == "shape")
        {

        }
        else
        {
            throw std::regex_error{std::regex_constants::error_type::_S_error_backref};
        }
        



        next_iterator++;
    }
}