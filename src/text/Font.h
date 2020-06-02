#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include <string>
#include <memory>
#include <exception>

#include "ftype.h"

#include "Face.h"

class Font {
    std::shared_ptr<unsigned char> buffer;
    FT_Long size;
    const char *error;

public:
    Font();
    Font(const std::string &path);
    // Checks for no error; that is, if `error` == nullptr and size > 0
    bool good();
    // returns `error`, equal to nullptr if no error
    const char *getError();
    Face mkFace(FT_Library library, FT_Error &error);
    // Throws `make_fail` upon failure
    Face make();
};

#endif
