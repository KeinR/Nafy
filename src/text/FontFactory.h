#ifndef FONTFACTORY_H_INCLUDED
#define FONTFACTORY_H_INCLUDED

#include <string>
#include <memory>
#include <exception>

#include "ftype.h"

#include "Font.h"

// Encapsulates raw font data and facilitates the conversion from
// that into Font instances

class FontFactory {
    // omg shared pointers are a fucking godsend
    std::shared_ptr<unsigned char> buffer;
    FT_Long size;
    const char *error;

public:
    FontFactory();
    FontFactory(const std::string &path);
    // Checks for no error; that is, if `error` == nullptr and size > 0
    bool good() const;
    // returns `error`, equal to nullptr if no error
    const char *getError() const;
    // Yes you WILL check that error!
    Font::type make(FT_Library library, FT_Error &error) const;
};

#endif
