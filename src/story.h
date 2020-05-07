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
        virtual void reset() = 0;
        virtual bool action(context *ctx) = 0;
    };

    // class textRules {
    //     typedef std::function<void()> func;
    //     std::unordered_map<char, func> letters;
    //     std::unordered_map<std::string, func> words;
    // };

    class textString: public storyEvent {
        std::string str;
        std::string::iterator ch;
    public:
        textString(const std::string &str);
        ~textString() override;
        void reset() override;
        bool action(context *ctx) override;
    };

    // class prompt: public storyEvent {
    //     std::string str;
    //     prompt(const std::string &str);
    //     void action() override;
    // };


    class scene {
        std::vector<storyEvent*> events;
        std::vector<storyEvent*>::size_type index;
    public:
        scene();
        ~scene();
        void pushText(const std::string &str);
        void pushEvent(const storyEvent &e);
        void reset();
        void run(context *ctx);
    };
}


#endif
