#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include <exception>
#include <string>

namespace nafy {
    // To group the other errors
    class nafy_error: public std::exception {
    public:
        virtual const char *what() const noexcept = 0;
    };

    // Normal error with a predefined message
    class error: public nafy_error {
        const char *message;
    public:
        error(const char *message) noexcept;
        const char *what() const noexcept override;
    };

    // Error with a dynamic error message
    class dy_error: public nafy_error {
        const std::string message;
    public:
        dy_error(const std::string &message) noexcept;
        const char *what() const noexcept override;
    };

    // Freetype error, thrown when an error was encountered in freetype.
    // Note that trivial errors are simply logged or ignored.
    class ft_error: public error {
    public:
        // Last typedef as per the docs, def'd here
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
