#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include <string>
#include <list>
#include <memory>

#include "glfw.h"

#include "story.h"
#include "Shader.h"
#include "ShaderFactory.h"
#include "TextLibrary.h"
#include "Rectangle.h"
#include "Color.h"
#include "View.h"
#include "Button.h"
#include "callback.h"

#include "text/ftype.h"
#include "text/TextCrawl.h"

// TODO: Memory manegement

namespace nafy {
    class context {
    public:
        struct views_s {
            // View 1: The first screen, the home screen
            struct home_s {
                View view;
                Color background;
                Face titleFace;
                Text title;
                Face buttonFace;
                Button startGame;
            } home;

            // View 2: The menu screen, brought up mid-game to view options
            // struct menu_s {
            //     View view;
            //     Color background;
            //     Face titleFace;
            //     Text topTitle;
            //     Button resumeGame;
            //     Button toMenu;
            //     // Button toConfig;
            // } menu;
        };
    private:
        GLFWwindow *window;

        // Text related stuff
        TextLibrary textLib;
        Font defaultFont;
        TextCrawl crawl;

        scene *root;
        scene *current;
        // int currentID;
        bool run;

        // std::vector<Render*> renders;
        float frameCooldown;


        // No purpose being here save to be retrieved by the user
        unsigned int framesPerSecond;
        int vsync;

        std::list<mouseMoveCallback*> cursorPosCallbacks;
        std::list<mouseClickCallback*> cursorButtonCallbacks;

        struct {
            shader_t sprite;
            shader_t prim;
        } defaultShaders;

        Color *background;
        View *view;

        // Dynamic because we have a lot of stuff
        std::shared_ptr<views_s> views;

        inline void makeCurrent();
    public:

        // Pointers towards `initCrawlFace` and `root` are stored
        // Throws `ft_error` if text setup failed
        context(int winWidth, int winHeight, const char *winTitle, scene &root);
        ~context();

        // Let's not bother with these for now
        context(const context &other) = delete;
        context(context &&other) = delete;
        context &operator=(const context &other) = delete;
        context &operator=(context &&other) = delete;

        void mousePosCallback(double x, double y);
        void mouseButtonCallback(int button, int action, int mods);

        void addMousePosCallback(mouseMoveCallback *callback);
        void addMouseButtonCallback(mouseClickCallback *callback);
        void removeMousePosCallback(mouseMoveCallback *callback);
        void removeMouseButtonCallback(mouseClickCallback *callback);

        void activate();

        void setRoot(scene &root);
        // Sets `this->current` to `current` and resets it
        void setCurrent(scene &current);
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
        Face makeFace(Font &font);

        void setDefaultSpriteShader(shader_t shader);
        void setDefaultPrimShader(shader_t shader);

        std::shared_ptr<views_s> getViews();

        /* private */
        shader_t getDefaultSpriteShader();
        shader_t getDefaultPrimShader();
        // Face *getDefFace();
        TextCrawl &getCrawl();
        Face &getCrawlFace();
        Face &getDefaultFace();
        bool shouldStop();

        void stopIfCurrent(scene *obj);
    };
}

#endif
