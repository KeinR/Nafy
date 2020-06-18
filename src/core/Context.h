#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include <string>
#include <list>
#include <memory>

#include "glfw.h"

#include "../game/Scene.h"
#include "../shaders/Shader.h"
#include "../gui/Rectangle.h"
#include "../gui/Color.h"
#include "../gui/View.h"
#include "../gui/Button.h"
#include "../gui/Image.h"
#include "../gui/EventDispatch.h"
#include "TextLibrary.h"

#include "../text/ftype.h"
#include "../text/TextCrawl.h"

/*
 * The context class, responsible for managing the entire program.
 * DO NOTE: Creating a context should be the FIRST thing you do
 * (aside from perhaps factories, because they're designed to be context-agnostic).
 * This is because many, many classes and functions operate depending on the _current_
 * context. This means that undefined behavior is invoked if you try operating one of those
 * classes or functions when a context isn't active, and that you have to be careful when
 * you have more than one context (which should be rather unlikely).
 * Do also note that starting the game before a root Scene is set will cause an `error` to be thrown.
 */

// TODO: Memory manegement
// TODO: Multible windows in one context

namespace nafy {
    class Context {
    public:

        struct views_s {
            // View 1: The first screen, the home screen
            struct home_s {
                View view;
                Color background;
                Text title;
                Button startGame;
            } home;

            // The run of the game itself, this is (hopefully) what the user
            // most of their time on
            struct game_s {
                View view;
                Color background;
                TextRec speaker;
                CrawlButton crawl;
            } game;

            // View 2: The menu screen, brought up mid-game to view options
            struct menu_s {
                View view;
                Rectangle bg;
                Text topTitle;
                Button resumeGame;
                Button toMenu;
                // Button toConfig;
            } menu;
        };
    private:
        // Main window
        GLFWwindow *window;

        EventDispatch dispatch;

        // Default shaders
        struct {
            shader_t sprite;
            shader_t prim;
        } defaultShaders;

        // Freetype library - analogous to a context
        TextLibrary textLib;
        // Font used by text in the absence of a provided one
        Font::type defaultFont;

        // The start scene
        Scene *root;
        // The current scene
        Scene *current;
        // Shoud the main loop run?
        bool run;
        // Should the scenes be processed?
        bool runGameAction;
        // Has the user triggered the next event (ie by clicking the text box, pressing enter...)
        bool userAdvance;

        // How long until the game loops next
        float frameCooldown;

        // Current background - the "clear color" that the screen is set to at the start of each frame
        Color *background;
        // The currently rendering view
        View *view;

        // Views and their renderables owned by this Context
        // Really just for convinience
        // Dynamic because we have a lot of stuff
        std::shared_ptr<views_s> views;

        // No purpose being here save to be retrieved by the user
        unsigned int framesPerSecond; // "FPS"
        int vsync; // "Vertical Sync"

        // Make all of this Context's bindings current
        void makeCurrent();
        // Run frame logic
        void runFrame();
    public:

        // Gives the initial width, height, and title of the window.
        // These can be changed later.
        Context(int winWidth, int winHeight, const char *winTitle);
        ~Context();

        // Let's not bother with these for now
        Context(const Context &other) = delete;
        Context(Context &&other) = default;
        Context &operator=(const Context &other) = delete;
        Context &operator=(Context &&other) = default;

        // When constructed, the context associates an event dispatch with
        // the main window's events, so that these functions get called on such events.
        // Of course, if you feel the need to simulate a key press etc, feel free to call
        // the dispatch'd overridden functions.
        EventDispatch &getDispatch();

        // Sets context to current.
        // This is done in the context constructor.
        // IMPORTANT IF YOU ARE USING MORE THAN ONE CONTEXT!!!!
        void activate();

        // Sets the starting scene.
        // MUST be done before game is started, else `error` is thrown
        void setRoot(Scene &root);
        // Sets `this->current` to `current` and initializes it
        void setCurrent(Scene &current);
        // Sets `current` to `root`
        void revert();
        // calls revert() and resume()
        void start();
        // Execute from `current`
        void resume();
        // Sets the running state of scene execution
        void setGameRunning(bool value);

        // Stop main game loop
        void stop();

        // Sets the current rendered view
        void setView(View &view);
        // Set the frame clear color
        void setBackground(Color &color);

        // Set/get the frames per second
        void setFPS(unsigned int fps);
        unsigned int getFPS();

        // VSync works by syncing to the monitors refresh rate.
        // This does mean however that it's dependent on the user's hardware.
        // Can reduce needless CPU utilization when waiting
        // for next frame.
        // Reccomended to go somewhere between 0-2
        // 1's generally a safe bet.
        // Some people really hate it though...
        void setVSync(int lv);
        int getVSync();

        // Throws `ft_error` if failed
        // Makes a new font based on this Context's TextLibrary
        Font::type makeFont(const FontFactory &factory);

        // Set defaults.
        // These will be grabbed when the respective shader/font isn't
        // provided to some class
        void setDefaultSpriteShader(const shader_t &shader);
        void setDefaultPrimShader(const shader_t &shader);
        void setDefaultFont(const Font::type &font);

        // Gets this context's owned views, by either
        // shared_ptr (reccomended) or reference, if you're
        // just doing something quick.
        std::shared_ptr<views_s> getViews();
        views_s &getViewsRef();

        // Shortcut for
        // views->game.speaker.getText().setString(name);
        // views->game.speaker.getText().generate();
        //, sets the displayed name in the little name box. 
        void setSpeaker(const std::string &name);

        /* private, but use if you must */

        // Default getters
        shader_t getDefaultSpriteShader();
        shader_t getDefaultPrimShader();
        Font::type getDefaultFont();

        // These two are really just for conviniences sake
        TextCrawl &getCrawl();
        View &getGameView();
        bool shouldStop();

        // Stop the scene execution (setGameRunning(false)) if `obj` is equal to `current`
        void stopIfCurrent(Scene *obj);

        // Sets/gets if user triggered the next event (ie by clicking the text box, pressing enter...) 
        // Used by classes that wait for user input (ie game/TextString)
        void setUserAdvance(bool value);
        bool getUserAdvance();
    };
}

#endif
