#include "error.h"

#include "../env/env.h"

nafy::error::error(const std::string &message) noexcept: message(message) {
}
const char *nafy::error::what() const noexcept {
    return message.c_str();    
}

nafy::error_c::error_c(const std::string &message, error_code code) noexcept: error(message), code(code) {
}
nafy::error_c::error_code nafy::error_c::getCode() const noexcept {
    return code;
}

nafy::ft_error::ft_error(error_code code) noexcept: error_c("FreeType Error", code) {
}
nafy::ft_error::ft_error(const std::string &message, error_code code) noexcept: error_c(message, code) {
}


nafy::gl_error::gl_error(error_code code) noexcept: error_c(getGLErrorStr(code), code) {
}
nafy::gl_error::gl_error(const std::string &message, error_code code) noexcept: error_c(message, code) {
}

nafy::al_error::al_error(error_code code) noexcept: error_c(getALErrorStr(code), code) {
}
nafy::al_error::al_error(const std::string &message, error_code code) noexcept: error_c(message, code) {
}
