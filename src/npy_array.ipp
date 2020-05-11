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
            uint16_t header_length_v1;
            array_stream.read(reinterpret_cast<char*>(&header_length_v1), 2);
            header_length = machine_endianess == npy_endianness::little_endian ? header_length_v1 : __bswap_16(header_length_v1);
        }
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
        else
        {
            throw npy_array_exception{npy_array_exception_type::unsupported_version};
        }
        
        // Reserve the required memory for storing the header and read it from the file.
        // If the memory reservation fails (very unlikely tho), it will throw a bad_alloc exception, which is caught.
        header.resize(header_length);
        array_stream.read(&header[0], header_length);

        // Parse the header of the file.
        // This function will assign a value to the following field: shape, fortran_order, byte_order, and item size.
        // If the header parse fails to assign a value to any of such fields, then this function will throw an exception.
        this->parse_header(header);

        this->data.resize(this->size());

        array_stream.read(reinterpret_cast<char*>(this->data.data()), this->byte_size());
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
void npy_array<T>::parse_header(const std::string& header)
{
    // Create a copy of the header string and remove from it the whitespace and new line characters.
    auto header_copy{header};
    header_copy.erase(std::remove_if(header_copy.begin(), header_copy.end(), [](char c){
        return std::isspace(c) || c == '\n';
    }), header_copy.end());

    //boost::regex pattern{R"('(\w+)':('([<>]\w+)'|\w+|\(\d+(?:,\d+|,)*\)))"};
    //'(\w+)'
    boost::regex pattern{R"((\w+))"}; // Regex pattern that catches any word enclosed by '.
    // The regex iterator that will iterate over all the matches in the header string.
    boost::sregex_iterator next_iterator{header_copy.begin(), header_copy.end(), pattern};
    // The end of a regex iterator is assumed to be the empty iterator.
    boost::sregex_iterator end_iterator{};
    // Mask that contains 3 flags, each flag is 0 if the header key has not been found, otherwise is 1.
    // Bit 0: descr, Bit 1: fortran_order, and Bit 2: shape.
    std::bitset<3> fields_mask{0};
    
    //
    while(next_iterator != end_iterator)
    {
        // If a match has been found, retrieve the match list and extract the first group which will contain the header key.
        const auto match_results = *next_iterator;

        // We ensure that the match is not empty and contains at least two sub matches:
        // the first match is the header key with the apices, like 'descr'.
        // the second match is the header key itself, like descr.
        if(match_results.empty() || match_results.size() < 2)
        {
            throw boost::regex_error{boost::regex_constants::error_unknown};
        }
        
        // Get the second match that is the header key.
        const auto key = match_results[1];

        if(key.str() == "descr")
        {
            boost::regex description_pattern{R"('(<\w+)')"};
            boost::match_results<std::string::const_iterator> description_match{};

            /*
            if(!boost::regex_search(key.second, header_copy.cend(), description_match))
            {

            }

            std::cout << description_match[0].str() << std::endl;
            */
            //const auto sub = header_copy.substr()
            //std::cout << key.first << " " << key.second << std::endl;

            /*
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
            */
        }
        /*
        else if(key == "fortran_order")
        {
            this->fortran_order = match[2].str() == "False" ? false : true;
            debug_message("NPY Array: Fortran Order: " << this->fortran_order);
        }
        else if(key == "shape")
        {
            auto shape_string = match[2].str();
            boost::regex shape_pattern{R"((\d+))"};
            boost::sregex_iterator shape_iterator{shape_string.begin(), shape_string.end(), shape_pattern};

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
            throw boost::regex_error{boost::regex_constants::error_backref};
        }
        */
        next_iterator++;
    }
}