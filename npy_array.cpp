#include "npy_array.h"

template<typename T>
npy_array<T>::npy_array(const std::string& array_path)
{
    std::string magic_string;
    uint8_t major_version;
    uint8_t minor_version;
    uint16_t header_length;
    std::string header;
    std::ifstream array_stream;

    array_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);

    debug_message("NPY Array: opening file path " << array_path);

    try
    {
        array_stream.open(array_path);

        debug_message("NPY Array: file opened successfully at path " << array_path);

        magic_string.resize(6);
        array_stream.read(&magic_string[0], 6);

        debug_message("NPY Array: magic string is " << magic_string);

        if(magic_string != "\x93NUMPY")
        {
            debug_message("NPY Array: magic string is incorrect");
            //TODO: lanciare eccezione perchè non ho riconosciuto la magic string.
        }

        array_stream.read(reinterpret_cast<char*>(&major_version), 1);
        array_stream.read(reinterpret_cast<char*>(&minor_version), 1);

        debug_message("NPY Array: version " << static_cast<int>(major_version) << "." << static_cast<int>(minor_version));

        if(major_version != 0x01)
        {
            debug_message("NPY Array: unsupported major version " << static_cast<int>(major_version));
            //TODO Lanciare eccezione
        }

        array_stream.read(reinterpret_cast<char*>(&header_length), 2);

        debug_message("NPY Array: header length " << header_length << " bytes");

        header.resize(header_length);
        array_stream.read(&header[0], header_length);

        debug_message("NPY Array: header '" << header << "'");

        this->parse_header(header);
    }
    catch(const std::ios_base::failure& failure_exception)
    {
        //TODO Lanciare eccezione perchè c'è stato un errore relativo alla lettura/apertura del file.
    }
    catch(const std::regex_error& regex_exception)
    {
        std::cout << regex_exception.what() << std::endl;
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