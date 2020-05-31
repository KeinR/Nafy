#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include <string>
#include <list>

#include "glfw.h"

#include "story.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "TextLibrary.h"
#include "Rectangle.h"

#include "text/ftype.h"
#include "text/TextCrawl.h"

// TODO: Memory manegement

namespace nafy {
    class context {
    public:
        // No virtual deconstructors because these
        // won't be seeing a delete
        class mouseMoveCallback {
        public:
            virtual void mouseMoved(double mouseX, double mouseY) = 0;
        };
        class mouseClickCallback {
        public:
            // Misleading I suppose, because it can be released too...
            virtual void mouseClicked(bool isPressed, int button, int mods) = 0;
        };
    private:
        GLFWwindow *window;

        // Text related stuff
        ShaderProgram textShader;
        TextLibrary textLib;
        Face crawlFace;
        TextCrawl crawl;

        scene *root;
        scene *current;
        int currentID;
        bool run;

        // std::vector<Render*> renders;
        float frameCooldown;

        Font defaultFont;
        ShaderProgram defaultShader;

        // No purpose being here save to be retrieved by the user
        unsigned int framesPerSecond;
        int vsync;

        std::list<mouseMoveCallback*> cursorPosCallbacks;
        std::list<mouseClickCallback*> cursorButtonCallbacks;

        inline void makeCurrent();
    public:

        // Pointers towards `initCrawlFace` and `root` are stored
        // Throws `ft_error` if text setup failed
        context(int winWidth, int winHeight, const char *winTitle, scene &root, Font &defaultFont, Shader &defaultShader);
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


        void setFPS(unsigned int fps);
        unsigned int getFPS();

        // Reccomended to go somewhere between 0-2
        // 1's generally a safe bet
        void setVSync(int lv);
        int getVSync();

        // Throws `ft_error` if failed
        Face makeFace(Font &font);

        /* private */
        ShaderProgram *getDefShader();
        Font *getDefFont();
        TextCrawl &getCrawl();
        Face &getCrawlFace();
        bool shouldStop();

        void stopIfCurrent(scene *obj);
    };
}

#endif
