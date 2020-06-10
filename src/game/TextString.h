#ifndef TEXTSTRING_H_INCLUDED
#define TEXTSTRING_H_INCLUDED

#include <string>

#include "sceneEvent.h"

namespace nafy {
    // Forward declartion to get around circular dependency
    class Scene;
    class Context;

    class TextString: public sceneEvent {
        std::string str;
        std::string::size_type index;
        float wait;
        float next;
        bool rolling;
    public:
        static unsigned int defaultCooldown;
        TextString(const std::string &str, unsigned int cooldownMillis = defaultCooldown);

        // Setting the string won't have any effect unil the next call
        // to init()... And in the meantime, it'll make the text crawl look
        // weird 
        void setString(const std::string &str);
        void setCooldown(unsigned int cooldownMillis);
        std::string getString();
        unsigned int getCooldown();

        void init(Context *ctx, Scene *parent) override;
        bool action(Context *ctx, Scene *parent) override;
    };
};

#endif
