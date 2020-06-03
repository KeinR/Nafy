#include "error.h"

nafy::error::error(const char *message) noexcept: message(message) {
}
const char *nafy::error::what() const noexcept {
    return message;    
}

nafy::dy_error::dy_error(const std::string &message) noexcept: message(message) {
}
const char *nafy::dy_error::what() const noexcept {
    return message.c_str();
}

nafy::ft_error::ft_error(const char *message, error_code code) noexcept: error(message), code(code) {
}
nafy::ft_error::error_code nafy::ft_error::getCode() const noexcept {
    return code;
}
