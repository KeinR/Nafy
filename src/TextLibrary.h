#ifndef TEXTLIBRARY_H_INCLUDED
#define TEXTLIBRARY_H_INCLUDED

#include <map>
#include <string>
#include <exception>

#include "error.h"

#include "text/ftype.h"
#include "text/Font.h"

// Note that FontLibrary is case insensitive

namespace nafy {
    class TextLibrary {
        FT_Library library;
    public:
        TextLibrary();
        ~TextLibrary();
        // Throws `ft_error` on failure
        Face makeFace(Font &font);
    };
}

#endif
