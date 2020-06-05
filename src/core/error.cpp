#include "error.h"

#include "../env/env.h"

nafy::error::error(const std::string &message) noexcept: message(message) {
}
const char *nafy::error::what() const noexcept {
    return message.c_str();    
}

nafy::ft_error::ft_error(const char *message, error_code code) noexcept: error(message), code(code) {
}
nafy::ft_error::error_code nafy::ft_error::getCode() const noexcept {
    return code;
}

nafy::gl_error::gl_error(error_code code) noexcept: error(getGLErrorStr(code)), code(code) {
}
nafy::gl_error::gl_error(const std::string &message) noexcept: error(message), code(0) {
}
nafy::gl_error::error_code nafy::gl_error::getCode() const noexcept {
    return code;
}
