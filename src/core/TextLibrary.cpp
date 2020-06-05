#include "TextLibrary.h"

#include <iostream>

#include "error.h"

nafy::TextLibrary::TextLibrary() {
    FT_Error error = FT_Init_FreeType(&library);
    if (error) {
        throw ft_error("Failed to init freetype library", error);
    }
}

nafy::TextLibrary::~TextLibrary() {
    FT_Done_FreeType(library);
}

void nafy::TextLibrary::delLib() {
    if (library != nullptr) {
        FT_Done_FreeType(library);
    }
}

void nafy::TextLibrary::steal(TextLibrary &other) {
    delLib();
    library = other.library;
    other.library = nullptr;
}

Font::type nafy::TextLibrary::makeFont(const FontFactory &factory) const {
    FT_Error error;
    Font::type font = factory.make(library, error);
    if (error) {
        throw ft_error("TextLibrary::makeFont failed", error);
    }
    return font;
}
