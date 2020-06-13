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

    // Generic error
    class error: public nafy_error {
        const std::string message;
    public:
        error(const std::string &message) noexcept;
        const char *what() const noexcept override;
    };

    // Error with an error code
    class error_c: public error {
    public:
        // Generally the size of error types that I've seen... Good enough
        typedef int error_code;
    private:
        const error_code code;
    public:
        error_c(const std::string &message, error_code code) noexcept;
        error_code getCode() const noexcept;
    };

    // Freetype error, thrown when an error was encountered in freetype.
    // Note that trivial errors are simply logged or ignored.
    class ft_error: public error_c {
    public:
        ft_error(error_code code) noexcept;
        ft_error(const std::string &message) noexcept;
        ft_error(const std::string &message, error_code code) noexcept;
    };

    // OpenGL error, thrown when, as you might expect, there was some
    // problem rendering something.
    // The errors that cause this to be thrown are generally rather serious.
    // ShaderFactories will throw these if they fail
    class gl_error: public error_c  {
    public:
        gl_error(error_code code) noexcept;
        gl_error(const std::string &message) noexcept;
        gl_error(const std::string &message, error_code code) noexcept;
    };

    // OpenAL error, or more broadly, something related to audio
    // Generally thrown when playback/streaming/loading fails
    class al_error: public error_c {
    public:
        al_error(error_code code) noexcept;
        al_error(const std::string &message) noexcept;
        al_error(const std::string &message, error_code code) noexcept;
    };
}

#endif
