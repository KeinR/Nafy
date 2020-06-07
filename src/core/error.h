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

    class error: public nafy_error {
        const std::string message;
    public:
        error(const std::string &message) noexcept;
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

    // OpenGL error, thrown when, as you might expect, there was some
    // problem rendering something.
    // The errors that cause this to be thrown are generally rather serious.
    // ShaderFactories will throw these if they fail
    class gl_error: public error  {
    public:
        // GLenum is 32 bit, so...
        typedef int error_code;
    private:
        error_code code;
    public:
        gl_error(error_code code) noexcept;
        gl_error(const std::string &message) noexcept;
        error_code getCode() const noexcept;
    };

    class al_error: public error {
    public:
        // GLenum is 32 bit, so...
        typedef int error_code;
    private:
        error_code code;
    public:
        al_error(error_code code) noexcept;
        al_error(const std::string &message) noexcept;
        error_code getCode() const noexcept;
    };
}

#endif
