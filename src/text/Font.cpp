#include "Font.h"

#include <algorithm>
#include <iostream>
#include <fstream>

#include "textdefs.h"

// TODO: Merge this with the parent repo

Font::Font(): size(0), error(nullptr) {
}

Font::Font(const std::string &path) {

    std::ifstream file(path);
    if (!file.good()) {
        error = "Failed to open file";
        size = 0;
        return;
    }

    file.seekg(0, file.end);
    const int length = file.tellg();
    file.seekg(0, file.beg);

    buffer.reset(new unsigned char[length]);

    file.read((char *)buffer.data(), length);

    if (!file.good()) {
        error = "Failed to read from file";
        // Bad data - delete to avoid other potential errors
        buffer.reset();
        size = 0;
        return;
    }

    file.close();

    size = sizeof(unsigned char) * length;
    error = nullptr;
}

bool Font::good() {
    return error == nullptr && size > 0;
}

const char *Font::getError() {
    return error;
}

Face Font::mkFace(FT_Library library, FT_Error &error) {
    FT_Face face;
    error = FT_New_Memory_Face(library, buffer, size, 0, &face);
    return Face(face);
}

Face Font::make() {
    return makeFace(*this);
}
