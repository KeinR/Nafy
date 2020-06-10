#ifndef BUTTONPROMPT_H_INCLUDED
#define BUTTONPROMPT_H_INCLUDED

#include <functional>

#include "sceneEvent.h"

#include "../gui/View.h"
#include "../gui/Button.h"
#include "../gui/renderable.h"

namespace nafy {
    class context;
    class Scene;

    class ButtonPrompt: public sceneEvent, public renderable {
    public:
        typedef std::vector<Button> elements_cont_t;
        typedef std::function<void()> callback_func;
        struct elementData {
            std::string str;
            callback_func func;
            elementData(const std::string &str, const callback_func &func);
        };
        enum com { done };
    private:
        View *view;
        Color color;
        Color colorLight;

        unsigned int cornerRadius;
        unsigned int spacing;
        unsigned int width;
        unsigned int height;
        unsigned int margin;

        elements_cont_t elements;
        bool added;
        bool shouldStop;
        void joinView();
        void leaveView();
        void activate();
        void deactivate();
        void steal(ButtonPrompt &other);
        void copy(const ButtonPrompt &other);
        void copyPOD(const ButtonPrompt &other);
    public:
        // Default color: light blue-ish. cornerRadius = 0. view = current context game view,
        // (context.views.game.view)
        ButtonPrompt();
        // If view nullptr, selects the view for the game view from the current context
        ButtonPrompt(const Color &color, unsigned int cornerRadius = 0, View *view = nullptr);
        ~ButtonPrompt() override;
        ButtonPrompt(ButtonPrompt &&other);
        ButtonPrompt(const ButtonPrompt &other);
        ButtonPrompt &operator=(ButtonPrompt &&other);
        ButtonPrompt &operator=(const ButtonPrompt &other);

        ButtonPrompt &setSpacing(unsigned int value);
        ButtonPrompt &setWidth(unsigned int value);
        ButtonPrompt &setHeight(unsigned int value);
        ButtonPrompt &setMargin(unsigned int value);

        unsigned int getSpacing();
        unsigned int getWidth();
        unsigned int getHeight();
        unsigned int getMargin();

        void setColor(const Color &color);
        // Removes from current view and adds to new one
        void setView(View &view);
        // Sets it for all current and future buttons
        void setCornerRadius(unsigned int cornerRadius);
        // Add a button
        void add(const std::string &str, const callback_func &func);
        void add(const elementData &data);
        // MUST BE DONE AT END AT ALL TIMES;
        // positions buttons and calls generate on them
        void generate();

        // Adds a button to the `elements` vector
        // NB: You must call generate() (or sent com::done to operator+)
        // in order to position and generate the buttons
        ButtonPrompt &operator+(const elementData &cb);
        // For convinience, calls generate() if given com::done
        ButtonPrompt &operator+(com command);

        // Gets the element container for lower level access
        elements_cont_t &getElements();


        void init(context *ctx, Scene *sc) override;
        bool action(context *ctx, Scene *sc) override;
        void render() override;
    };
}

#endif
