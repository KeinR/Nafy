#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <string>
#include <memory>

#include "../shaders/Shader.h"
#include "../render/Buffer.h"
#include "../render/Texture.h"
#include "../render/Model.h"

#include "renderable.h"

/*
* Image, a renderable node that displays the content of a bitmap.
* Data given to an Image though setImage(...) is copied to an OpenGL
* texture, so no need to keep such data allocated.
* Since all images use the same verticies, indices and vertex objects,
* a global buffer is cached and re-used by all image objects.
* 
* There are other functions present here as well that allow you to simply
* load an image as a bitmap.
* There are two ways of accomplishing this:
* The first way involves the load function storing in the width, height,
* and format parameters their corresponding values, and returning a bitmap.
* This bitmap MUST be deallocated with freeImage(...), because stb_image is a
* C lib and it allocates with malloc etc. While it does give the option to change
* the defs for malloc, realloc and free, I don't want to mess with it at this moment.
* The second way is the (IMO) preferred way, which takes a string and returns a shared
* pointer for a struct containing the image data. The shared pointer has a custom
* deleter that frees the image data (`data`) with freeImage(...), so that you don't
* have to worry about doing it yourself.
*/

namespace nafy {

    // Gets an OpenGL image format from the number of
    // given image channels.
    // Supports 1-4 incl. channels.
    // 1 = GL_RED
    // 2 = GL_RG
    // 3 = GL_RGB
    // 4 = GL_RGBA
    // Returns 0 if the format is not supported,
    // ie `channels` is <= 0 and > 4.
    // Worth noting that `0` shouln't conflict with any of the
    // above formats's values.
    // Perhaps it should return a reserved OpenGL enum instead? TODO
    int getImageFormat(int channels);

    // Struct returned by loadImageStr(...), wrapped in a shared pointer
    struct imageData {
        int format;
        int width;
        int height;
        unsigned char *data;
    };

    // The type that's returned by loadImageStr(...)
    typedef std::shared_ptr<imageData> man_image;

    // The next two functions load a image from the given path.
    // Both will throw an instance of `error` upon failure,
    // either because of an i/o problem, or the channel # is unsupported

    // Loads an image from the given path and returns the bitmap.
    // metadata is put in the `width`, `height`, and `format` parameters.
    // If an error is thrown, the aforementioned metadata parameters are not changed.
    // NOTE WELL: The image data returned by the function MUST be deallocated with freeImage(...)!
    unsigned char *loadImagePath(const std::string &path, int &width, int &height, int &format);
    // Loads an image from the given path and returns a shared pointer to a struct containing
    // the image data, unlike the previous function.
    // The shared pointer will have a custrom destructor that will free the image data with
    // freeImage(...), so you don't have to worry about that.
    man_image loadImageStr(const std::string &path);

    // Frees the image data according to how it was allocated.
    // As of current, this would call stbi_free(...), which
    // calls a macro that is by default def'd to the C free(...)
    void freeImage(unsigned char *data);

    class Image: public renderable {
        // Holds reference to the cache so that
        // it doesn't deallocate.
        // Used to do the actual rendering
        std::shared_ptr<Buffer> buffer;

        // Holds the raw image data
        Texture texture;
        // Controls where the image is positioned
        Model model;
        // Specifies the used shader
        shader_t shader;

        // Initializes the image with the given shader.
        // Used as a sane way to handle multible constructors
        void init(const shader_t &initShader);
    public:
        // When first construced, the image will, as you might expect, not
        // have any image.

        // Default initialization, takes default shader from the current Context,
        // and the default texture param.
        Image();
        // Same as above, but this time specify a parameter for the texture
        Image(const Texture::tparam &texParams);
        // Same as the first, however specifying a shader
        // Shader requirements: model & sampler0
        Image(const shader_t &shader);
        // Opposate of the first, specify the texture parameter, as well as the shader.
        // Shader requirements: model & sampler0
        Image(const Texture::tparam &texParams, const shader_t &shader);

        // Loads an image from the given path and copies it into the texture.
        // Calls loadImageStr(...) with the `path` parameter,
        // so an instance of `error` will be thrown upon failure.
        // See loadImageStr(...) for more details.
        void loadImage(const std::string &path);
        // Copies the given imageData struct into the texture
        void setImage(const imageData &image);
        // Copies the data managed by the shared pointer into the texture
        void setImage(const man_image &image);
        // Copies the given data into the texture
        void setImage(int format, int width, int height, unsigned char *data);

        // Sets `shader` as the in-use shader for this image and initialize it.
        // Shader requirements: model & sampler0
        void bindShader(const shader_t &shader);
        // Gets the currently in use shader
        shader_t getShader();

        // Set position and width
        void setX(int x);
        void setY(int y);
        void setWidth(unsigned int width);
        void setHeight(unsigned int height);

        // Get position and width
        int getX();
        int getY();
        unsigned int getWidth();
        unsigned int getHeight();

        // Get the wrapped texture
        Texture &getTexure();

        // Render the image to the current window context
        void render() override;
    };
};

#endif
