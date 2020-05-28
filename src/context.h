#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include <string>

#include "glfw.h"

#include "story.h"
#include "FontLibrary.h"

#include "text/ftype.h"
#include "text/TextCrawl.h"

namespace nafy {
    class context {
        GLFWwindow *window;

        scene *root;
        scene *current;
        int currentID;
        bool run;

        // Text related stuff
        FT_Library ftLibrary;

        Face *crawlFace;
        TextCrawl crawl;

        shader_t textShader;

        // std::vector<Render*> renders;
        float frameCooldown;

        // No purpose being here save to be retrieved by the user
        unsigned int framesPerSecond;
        int vsync;

        inline void makeCurrent();
    public:

        // Pointers towards `initCrawlFace` and `root` are stored
        // Throws `ft_error` if text setup failed
        context(int winWidth, int winHeight, const char *winTitle, scene &root, Font &initCrawlFont, shader_t textShader = shaders::text);
        ~context();
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

        void stopIfCurrent(scene *obj);
    };

    // Please, use the constructor instead
    context makeContext(int width = 400, int height = 400, const char *title = "Nafy", scene *root);
}

#endif
