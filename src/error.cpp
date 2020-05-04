#include "error.h"

nafy::error::error(const char *message): message(message) {
}

const char *nafy::error::what() const noexcept {
    return message;    
}
