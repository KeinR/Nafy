#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include <string>

#include "ftype.h"

#include "Face.h"

class Font {
    unsigned char *buffer;
    FT_Long size;
    const char *error;
    
    inline void copy(const Font &other);
    inline void steal(Font &other);
    inline void delBuffer();
public:
    Font(const std::string &path);
    Font(const Font &other);
    Font();
    Font(Font &&other);
    Font &operator=(const Font &other);
    Font &operator=(Font &&other);
    ~Font();
    // Checks for no error; that is, if `error` == nullptr
    bool good();
    // returns `error`, equal to nullptr if no error
    const char *getError();
    Face mkFace(FT_Library library, FT_Error &error);
};

#endif
