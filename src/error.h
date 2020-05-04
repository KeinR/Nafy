#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include <exception>

namespace nafy {
    class error: public std::exception {
        const char *message;
    public:
        error(const char *message);
        const char *what() const noexcept override;
    };
}

#endif
