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
    std::cout << "try...." << std::endl;
    FT_Done_FreeType(library);
    std::cout << "done!" << std::endl;
}

Face nafy::TextLibrary::makeFace(Font &font) {
    FT_Error error;
    Face face = font.mkFace(library, error);
    if (error) {
        throw ft_error("TextLibrary::makeFace failed", error);
    }
    return face;
}
