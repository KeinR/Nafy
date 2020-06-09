#ifndef TEXTSTRING_H_INCLUDED
#define TEXTSTRING_H_INCLUDED

#include <string>

#include "sceneEvent.h"

namespace nafy {
    // Forward declartion to get around circular dependency
    class Scene;
    class context;

    class TextString: public sceneEvent {
        std::string str;
        std::string::size_type index;
        float wait;
        float next;
    public:
        static unsigned int defaultCooldown;
        TextString(const std::string &str, unsigned int cooldownMillis = defaultCooldown);
        ~TextString() = default;

        // Setting the string won't have any effect unil the next call
        // to init()... And in the meantime, it'll make the text crawl look
        // weird 
        void setString(const std::string &str);
        void setCooldown(unsigned int cooldownMillis);

        void init(context *ctx, Scene *parent) override;
        bool action(context *ctx, Scene *parent) override;
    };
};

#endif
