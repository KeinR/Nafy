#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include <exception>

namespace nafy {
    class error: public std::exception {
        const char *message;
    public:
        error(const char *message) noexcept;
        const char *what() const noexcept override;
    };

    // Freetype error, thrown when an error was encountered in freetype.
    // Note that trivial errors are simply logged or ignored.
    class ft_error: public error {
    public:
        // Last typedef as per the docs, defined here
        // so that we don't have to include the bulky freetype header
        typedef int error_code;
    private:
        const error_code code;
    public:
        ft_error(const char *message, error_code code) noexcept;
        error_code getCode() const noexcept;
    };
}

#endif
