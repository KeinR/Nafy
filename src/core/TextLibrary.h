#ifndef TEXTLIBRARY_H_INCLUDED
#define TEXTLIBRARY_H_INCLUDED

#include <map>
#include <string>
#include <exception>

#include "error.h"

#include "../text/ftype.h"
#include "../text/FontFactory.h"
#include "../text/Font.h"

namespace nafy {
    class TextLibrary {
        FT_Library library;
        inline void delLib();
        inline void steal(TextLibrary &other);
    public:
        TextLibrary();
        TextLibrary(const TextLibrary &other) = delete;
        TextLibrary(TextLibrary &&other);
        ~TextLibrary();
        TextLibrary &operator=(const TextLibrary &other) = delete;
        TextLibrary &operator=(TextLibrary &&other);
        // Throws `ft_error` on failure
        Font::type makeFont(const FontFactory &factory) const;
    };
}

#endif
