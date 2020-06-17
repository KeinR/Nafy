#ifndef FONTFACTORY_H_INCLUDED
#define FONTFACTORY_H_INCLUDED

#include <string>
#include <memory>
#include <exception>

#include "ftype.h"

#include "Font.h"

/*
* Encapsulates raw font data and facilitates the conversion from
* that into Font instances.
*/

class FontFactory {
    // The raw font data
    std::shared_ptr<unsigned char> buffer;
    // Size of the buffer
    FT_Long size;
    // The set error message
    const char *error;
public:
    // Default initialize - will be ill-formed if this is done!
    FontFactory();
    // Load from path - if an error occurred, getError() will contain
    // an error message, and good() will return false.
    FontFactory(const std::string &path);
    // Checks for no error; that is, if `error` == nullptr and size > 0
    bool good() const;
    // returns `error`, equal to nullptr if no error
    const char *getError() const;
    // Make a font, with `library` as the parent
    Font::type make(FT_Library library, FT_Error &error) const;
};

#endif
