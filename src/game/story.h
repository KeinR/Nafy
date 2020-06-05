#ifndef STORY_H_INCLUDED
#define STORY_H_INCLUDED

#include <vector>
#include <string>
// #include <unordered_map>



namespace nafy {
    class context;
    class scene;

    class storyEvent {
    public:
        storyEvent() = default;
        virtual ~storyEvent() = 0;
        virtual void init(context *ctx, scene *parent) = 0; // Called once
        // Return true if parent should move on to next event
        // Return false to be called again next frame
        virtual bool action(context *ctx, scene *parent) = 0; // Called every frame
    };

    // class textRules {
    //     typedef std::function<void()> func;
    //     std::unordered_map<char, func> letters;
    //     std::unordered_map<std::string, func> words;
    // };

    class textString: public storyEvent {
        std::string str;
        std::string::iterator ch;
        float wait;
        float next;
    public:
        textString(const std::string &str, unsigned int cooldownMillis);
        ~textString() override;
        void init(context *ctx, scene *parent) override;
        bool action(context *ctx, scene *parent) override;
    };

    // class prompt: public storyEvent {
    //     std::string str;
    //     prompt(const std::string &str);
    //     void action() override;
    // };

    class scene {
        struct event {
            storyEvent *ptr;
            bool managed; // That is, managed by this object
        };
        std::vector<event> events;
        std::vector<event>::size_type index;
    public:
        scene();
        ~scene();
        // Don't remove anything if currently running UNLESS you also call reset(),
        // otherwise undefined behavior gets invoked
        void pushText(const std::string &str, unsigned int cooldownMillis = 400);
        void pushEvent(storyEvent &e);
        void init(context *ctx);
        void run(context *ctx);
    };
}


#endif
