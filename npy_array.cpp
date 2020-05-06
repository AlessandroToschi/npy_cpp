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
    // The header length in bytes.
    // Version 1 header length is 2 bytes, Version 2 header length is 4 bytes.
    uint32_t header_length;
    // The header of the file is a Python dictionary represented as string which contains three fields:
    // 'desc': the NumPy.dtype object representation.
    // 'fortran_order': true if the values in the array are stored in fortran order, otherwise false.
    // 'shape': the shape of the array.
    // The header is terminated by a '\n' and padded with spaces ('\x20') to make the whole header even divisible by 64.
    std::string header;
    // The input stream object that is used to read the file.
    std::ifstream array_stream;

    // Get the endianess of this system.
    endianness machine_endianess = get_endianess();

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
            uint16_t header_length_v1;
            array_stream.read(reinterpret_cast<char*>(&header_length_v1), 2);
            header_length = machine_endianess == endianness::little_endian ? header_length_v1 : __bswap_16(header_length_v1);
        }
        // If the version is 2, then the header length is 4 bytes.
        // It is directly read in the header_length of the function.
        // The same consideration for the endianess applies for the version 2 header length.
        else if(major_version == 0x02)
        {
            array_stream.read(reinterpret_cast<char*>(&header_length), 4);

            if(machine_endianess == endianness::big_endian)
            {
                header_length = __bswap_32(header_length);
            }
        }
        // Version 3 still not supported or invalid version found, throw an exception.
        else
        {
            throw npy_array_exception{npy_array_exception_type::unsupported_version};
        }

        header.resize(header_length);
        array_stream.read(&header[0], header_length);

        this->parse_header(header);

        this->data.resize(this->size());

        array_stream.read(reinterpret_cast<char*>(this->data.data()), this->byte_size());
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

    std::regex pattern{R"('(\w+)':('([<>]\w+)'|\w+|\(\d+(?:,\d+|,)*\)))"};
    std::sregex_iterator next_iterator{header_copy.begin(), header_copy.end(), pattern};
    std::sregex_iterator end_iterator{};
    
    while(next_iterator != end_iterator)
    {
        const auto match = *next_iterator;
        const auto key = match[1].str();

        if(key == "descr")
        {
            const auto description = match[3].str();
            this->byte_order = get_endianess();
            this->data_size = std::stoull(description.substr(2));

            if(static_cast<char>(this->byte_order) != description[0])
            {
                throw "big endianess not supported";
            }

            if(sizeof(T) > this->data_size)
            {
                throw "type not valid for this anpy";
            }

            debug_message("NPY Array: Endianess: " << static_cast<char>(this->byte_order) << " Data size: " << this->data_size);
        }
        else if(key == "fortran_order")
        {
            this->fortran_order = match[2].str() == "False" ? false : true;
            debug_message("NPY Array: Fortran Order: " << this->fortran_order);
        }
        else if(key == "shape")
        {
            auto shape_string = match[2].str();
            std::regex shape_pattern{R"((\d+))"};
            std::sregex_iterator shape_iterator{shape_string.begin(), shape_string.end(), shape_pattern};

            while(shape_iterator != end_iterator)
            {
                const auto shape_match = *shape_iterator;
                this->shape.push_back(std::stoull(shape_match[0].str()));
                shape_iterator++;
            }

            debug_message("NPY Array: shape size: " << this->size());
        }
        else
        {
            throw std::regex_error{std::regex_constants::error_backref};
        }

        next_iterator++;
    }
}