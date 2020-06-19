#ifndef BUTTONPROMPT_H_INCLUDED
#define BUTTONPROMPT_H_INCLUDED

#include <functional>

#include "sceneEvent.h"

#include "../gui/View.h"
#include "../gui/Button.h"
#include "../gui/EventDispatch.h"
#include "../gui/renderable.h"

/*
* Event that prompts the user to press one of an array of buttons.
* Each button is associated with a user-provided function, and when pressed
* that function will be called.
* You can set global parameters for the buttons, and also edit them individually
* through getElements().
* Adds itself to a user provided view or the currently displaying one in the current context.
* Has ease-of-use functions that makes the creation process flow a little more.
*/

namespace nafy {
    class Context;
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
        EventDispatch dispatch;

        // Global settings, set every generate()
        unsigned int cornerRadius;
        // Spacing is the vertical distance between two consecutive buttons
        unsigned int spacing;
        unsigned int width;
        unsigned int height;
        unsigned int margin;
        Color color;
        Color colorLight;

        elements_cont_t elements;
        // Has this ButtonPrompt been added to `view`?
        bool added;
        // Checked every action(...), signifies that the button has been pressed
        // and that the ButtonPrompt should deactivate()
        // Checked every action(...) because events are dispatched concurrently,
        // so to prevent issues with that we just have a flag that triggers cleanup
        // on (what I assume is) the main thread.
        bool shouldStop;
        // Add self to a view
        void joinView();
        // Remove self from a view
        void leaveView();
        // joinView() and enable buttons (TODO: That's dumb)
        void activate();
        // leaveView() and disable buttons (TODO: That's dumb)
        void deactivate();
        void steal(ButtonPrompt &other);
        void copy(const ButtonPrompt &other);
        // Copy "Plain Old Data"; data such as ints, bools, etc that can't really be moved
        void copyPOD(const ButtonPrompt &other);
    public:
        // Default color: light blue-ish. cornerRadius = 0. view = current context game view,
        // (context.views.game.view)
        ButtonPrompt();
        // If view nullptr, selects the view for the game view from the current context
        ButtonPrompt(const Color &color, unsigned int cornerRadius = 0, EventDispatch *source = nullptr);

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
        void setDispatch(EventDispatch &source);
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

        void init(Context *ctx, Scene *sc) override;
        // Returns true as soon as a button is pressed
        bool action(Context *ctx, Scene *sc) override;
        // Renders the buttons
        void render() override;
    };
}

#endif
