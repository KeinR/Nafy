#ifndef TEXTSTRING_H_INCLUDED
#define TEXTSTRING_H_INCLUDED

#include <string>

#include "sceneEvent.h"

/*
* Text string initiates a text crawl on the `crawl` of the given Context.
* It sets the text content to `str`, and calls generate(). 
* It will then call advance() on the `crawl` every so often as specified in the
* constructor.
* If the user has prompted an advance (Context.getUserAdvance()), it advances the
* text until it's fully displayed.
* If the text is fully displayed, it waits for an advance (Context.getUserAdvance())
* before returning true to action(...), ending its event.
* I think it goes without saying, but this is not really thread safe.
*/

namespace nafy {
    // Forward declartion to get around circular dependency
    class Scene;
    class Context;

    class TextString: public sceneEvent {
        // The string to display in the TextCrawl `crawl`
        std::string str;
        // The current string index, used as a reference so that whitespace can
        // be ignored in the `wait` counter
        std::string::size_type index;
        // The base wait between calls to advance()
        float wait;
        // The time (seconds) at which the next call to advance() on the Context's `crawl` will
        // happen.
        float next;
        // If there's still text to display; the text is still crawling on screen.
        bool rolling;
    public:
        static unsigned int defaultCooldown;
        TextString(const std::string &str, unsigned int cooldownMillis = defaultCooldown);

        // Setting the string won't have any effect unil the next call
        // to init()... And in the meantime, it'll make the text crawl look
        // weird because of how the string is used as a reference for how fast
        // the text should be displayed... Shouldn't cause any bounds errors, though.
        void setString(const std::string &str);
        // Sets the base wait time between calls to advance() in milliseconds
        void setCooldown(unsigned int cooldownMillis);
        // Gets the currently used string
        std::string getString();
        // Gets the curret cooldown in milliseconds
        unsigned int getCooldown();

        // Sets `ctx`'s `crawl` text content to `str` and prepares the class
        void init(Context *ctx, Scene *parent) override;
        // Run class action
        bool action(Context *ctx, Scene *parent) override;
    };
};

#endif
