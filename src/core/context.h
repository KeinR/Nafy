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
#include "callback.h"
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
 */

// TODO: Memory manegement
// TODO: Multible windows in one context

namespace nafy {
    class context {
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
            // struct menu_s {
            //     View view;
            //     Color background;
            //     Text topTitle;
            //     Button resumeGame;
            //     Button toMenu;
            //     // Button toConfig;
            // } menu;
        };
    private:
        GLFWwindow *window;

        struct {
            shader_t sprite;
            shader_t prim;
        } defaultShaders;

        // Text related stuff
        TextLibrary textLib;
        Font::type defaultFont;

        Scene *root;
        Scene *current;
        // int currentID;
        bool run;
        bool runGameAction;
        bool userAdvance;

        float frameCooldown;

        // TODO: Change to vector for faster iteration
        std::vector<mouseMoveCallback*> cursorPosCallbacks;
        std::vector<mouseClickCallback*> cursorButtonCallbacks;

        Color *background;
        View *view;

        // Dynamic because we have a lot of stuff
        std::shared_ptr<views_s> views;

        // No purpose being here save to be retrieved by the user
        unsigned int framesPerSecond;
        int vsync;

        inline void makeCurrent();
        void runFrame();
    public:

        // Pointers towards `initCrawlFace` and `root` are stored
        // Throws `ft_error` if text setup failed
        context(int winWidth, int winHeight, const char *winTitle);
        ~context();

        // Let's not bother with these for now
        context(const context &other) = delete;
        context(context &&other) = delete;
        context &operator=(const context &other) = delete;
        context &operator=(context &&other) = delete;

        void mousePosCallback(double x, double y);
        void mouseButtonCallback(int button, int action, int mods);

        // Returns const iterators to the positions of the callbacks,
        // which can then later be used for fast removal 
        void addMousePosCallback(mouseMoveCallback &callback);
        void addMouseButtonCallback(mouseClickCallback &callback);
        void removeMousePosCallback(mouseMoveCallback *callback);
        void removeMouseButtonCallback(mouseClickCallback *callback);

        // Sets context to current
        // IMPORTANT IF YOU ARE USING MORE THAN ONE CONTEXT!!!!
        void activate();

        void setRoot(Scene &root);
        // Sets `this->current` to `current` and resets it
        void setCurrent(Scene &current);
        // Sets `current` to `root`
        void revert();
        // calls revert() and resume()
        void start();
        // Start from `current`
        void resume();
        void stop();

        void setView(View &view);
        void setBackground(Color &color);

        void setFPS(unsigned int fps);
        unsigned int getFPS();

        // Can reduce needless CPU utilization when waiting
        // for next frame.
        // Dependent on the user's hardware, however.
        // Reccomended to go somewhere between 0-2
        // 1's generally a safe bet
        void setVSync(int lv);
        int getVSync();

        // Throws `ft_error` if failed
        Font::type makeFont(const FontFactory &factory);

        void setDefaultSpriteShader(const shader_t &shader);
        void setDefaultPrimShader(const shader_t &shader);
        void setDefaultFont(const Font::type &font);

        std::shared_ptr<views_s> getViews();
        views_s &getViewsRef();

        void setSpeaker(const std::string &name);

        /* private */
        shader_t getDefaultSpriteShader();
        shader_t getDefaultPrimShader();
        Font::type getDefaultFont();
        TextCrawl &getCrawl();
        bool shouldStop();

        void setGameRunning(bool value);
        void stopIfCurrent(Scene *obj);
        void setUserAdvance(bool value);
        bool getUserAdvance();
    };
}

#endif
