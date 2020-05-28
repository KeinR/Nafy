#include "FontLibrary.h"

static void toLowercase(std::string &str) {
    for (char &c : str) {
        if (c >= 'A' && c <= 'Z') {
            c += 0x20;
        }
    }
    return str;
}

nafy::FontLibrary::FontLibrary() {
}

nafy::FontLibrary::container &nafy::FontLibrary::getContainer() {
    return fonts;
}

Font &nafy::FontLibrary::get(const std::string &key) {
    container::iterator element = fonts.find(toLowercase(key));
    if (element != fonts.end()) {
        return *element;
    }
    throw not_found();
}

void nafy::add(const std::string &key, const Font &value) {
    fonts[toLowercase(key)] = value;
}

void add(const std::string &key, Font &&value) {
    fonts[toLowercase(key)] = std::move(value);
}

void nafy::load(const std::string &key, const std::string &path) {
    fonts[toLowercase(key)] = Font(path);
}

const char *nafy::not_found::what() const noexcept {
    return "The given key has no matches";
}
