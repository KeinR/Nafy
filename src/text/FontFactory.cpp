#include "FontFactory.h"

#include <algorithm>
#include <iostream>
#include <fstream>

#include "textdefs.h"

// TODO: Merge this with the parent repo

FontFactory::FontFactory(): size(0), error(nullptr) {
}

FontFactory::FontFactory(const std::string &path) {

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

    file.read((char *)buffer.get(), length);

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

bool FontFactory::good() const {
    return error == nullptr && size > 0;
}

const char *FontFactory::getError() const {
    return error;
}

Font::type FontFactory::make(FT_Library library, FT_Error &error) const {
    FT_Face face;
    error = FT_New_Memory_Face(library, buffer.get(), size, 0, &face);
    return std::make_shared<Font>(face, buffer);
}
