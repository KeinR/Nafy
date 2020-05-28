
#include <map>
#include <string>
#include <exception>

#include "Font.h"

// Note that FontLibrary is case insensitive

namespace nafy {
    class FontLibrary {
    public:
        typedef std::map<std::string, Font> container;
    private:
        container fonts;
    public:
        class not_found: public std::exception {
        public:
            const char *what() const noexcept override;
        };
        // Gets the used container holding all the fonts
        container &getContainer();
        // Throws an instance of `FontLibrary::not_found` if the font doesn't exist
        Font &get(const std::string &key);
        // Will overwrite any pre-existing fonts
        void add(const std::string &key, const Font &value);
        void add(const std::string &key, Font &&value);
        // Load font from disk and add it to the library.
        // Analogous to emplace
        void load(const std::string &key, const std::string &path);
    };
}

