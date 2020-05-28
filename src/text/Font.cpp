#include "Font.h"

#include <algorithm>
#include <iostream>
#include <fstream>

// TODO: Merge this with the parent repo

Font::Font(const std::string &path) {

    std::ifstream file(path);
    if (!file.good()) {
        error = "Failed to open file";
        buffer = nullptr;
        size = 0;
        return;
    }

    file.seekg(0, file.end);
    const int length = file.tellg();
    file.seekg(0, file.beg);

    buffer = new unsigned char[length];

    file.read((char *)buffer, length);

    if (!file.good()) {
        error = "Failed to read from file";
        // Bad data - delete to avoid other potential errors
        delete[] buffer;
        // Could use gotos to conform better to DRY, however
        // I think I'll pass...
        buffer = nullptr;
        size = 0;
        return;
    }

    file.close();

    size = sizeof(unsigned char) * length;
    error = nullptr;
}

Font::Font(): buffer(nullptr), size(0), error(nullptr) {
}

Font::~Font() {
    delBuffer();
}

// Mem manegment

void Font::delBuffer() {
    if (buffer != nullptr) {
        delete[] buffer;
    }
}

void Font::copy(const Font &other) {
    size = other.size;
    delBuffer();
    if (other.buffer != nullptr) {
        const std::size_t length = other.size / sizeof(unsigned char);
        buffer = new unsigned char[length];
        std::copy(other.buffer, other.buffer + length, buffer);
    } else {
        buffer = nullptr;
    }
    error = other.error;
}

void Font::steal(Font &other) {
    size = other.size;
    delBuffer();
    buffer = other.buffer;
    other.buffer = nullptr;
    error = other.error;
}

Font::Font(const Font &other) {
    copy(other);
}

Font::Font(Font &&other) {
    steal(other);
}

Font &Font::operator=(const Font &other) {
    copy(other);
    return *this;
}

Font &Font::operator=(Font &&other) {
    steal(other);
    return *this;
}

// End mem manegment

bool Font::good() {
    return error == nullptr;
}

const char *Font::getError() {
    return error;
}

Face Font::mkFace(FT_Library library, FT_Error &error) {
    FT_Face face;
    error = FT_New_Memory_Face(library, buffer, size, 0, &face);
    return Face(face);
}
