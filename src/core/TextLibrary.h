#ifndef TEXTLIBRARY_H_INCLUDED
#define TEXTLIBRARY_H_INCLUDED

#include <map>
#include <string>
#include <exception>

#include "error.h"

#include "../text/ftype.h"
#include "../text/FontFactory.h"
#include "../text/Font.h"

/*
* Wrapps a Freetype library and provides facilities for making
* new fonts from it.
* Libraries are Freetype's version of a context: each one controls the objects spawned off of
* it, and their memory(?).
*/

namespace nafy {
    class TextLibrary {
        FT_Library library;
        inline void delLib();
        inline void steal(TextLibrary &other);
    public:
        TextLibrary();
        TextLibrary(TextLibrary &&other);
        ~TextLibrary();
        TextLibrary &operator=(TextLibrary &&other);
        // Throws `ft_error` on failure
        Font::type makeFont(const FontFactory &factory) const;
    };
}

#endif
