#include <exception>

enum npy_array_exception_type
{
    input_output_error,
    ill_formed_header,
    unsupported_version,
    invalid_magic_string,
    unsufficient_memory,
    generic
};

class npy_array_exception : std::exception
{
public:
    npy_array_exception(const npy_array_exception_type exception_type);

    const char* what();

    npy_array_exception_type exception_type() const;

private:
    npy_array_exception_type _exception_type;
};