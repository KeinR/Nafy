#include "TextLibrary.h"

#include <iostream>

#include "error.h"

nafy::TextLibrary::TextLibrary() {
    std::cout << "Start make lib" << std::endl;
    FT_Error error = FT_Init_FreeType(&library);
    if (error) {
        throw ft_error("Failed to init freetype library", error);
    }
}

nafy::TextLibrary::~TextLibrary() {
    std::cout << "try...." << std::endl;
    FT_Done_FreeType(library);
    std::cout << "done!" << std::endl;
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
    std::cout << "Make font" << std::endl;
    FT_Error error;
    Font::type font = factory.make(library, error);
    if (error) {
        throw ft_error("TextLibrary::makeFont failed", error);
    }
    std::cout << "Success!" << std::endl;
    return font;
}
