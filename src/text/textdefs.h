#ifndef TEXTDEFS_H_INCLUDED
#define TEXTDEFS_H_INCLUDED

/*
* Text configurations.
* Include what is needed in order to make compatable
*/

// The name of the mat4 model and sampler2D in the text shader(s)
// that will be given to the Text class.
// I realized actually, you know, that it wouldn't be too bad of
// an idea to just require an extension of some pure-virtual class
// as the shaders, with methods to get the below... The only problem
// being that such classes would be more or less singletons :/
// I'm still a little _meh_ about the idea of wrapping shaders in classes.
// I suppose it satisfies some OO princibles or something, but it's just extra
// bloat...
// This'll do for now. Can always change in the future ;)
// #define SHADER_MODEL "model"
// #define SHADER_TEXT_SAMPLER "Tex"

// glfw include header
#define GLFW_INCLUDE_HEADER_LOCATION "../core/glfw.h"

// Functions that you yourself must define

// Get the width and height of the context window
// void getWindowSize(int *width, int *height);

// Alias your functions to the needed ones
#include "../env/env.h"
static constexpr void(*getWindowSize)(int *width, int *height) = nafy::getWindowSize;


// Convert screen coords to GL quads
// float normX(float x);
// float normY(float y);

// Alias your functions to the needed ones
// #include "util.h" // Redundant
static constexpr float(*normX)(float x) = nafy::normX;
static constexpr float(*normY)(float y) = nafy::normY;


// For default initialization

#include "Font.h"
// #include "../env/env.h" // Redundant
static Font::type (*getDefaultFont)() = []() -> Font::type {
    return nafy::getContext()->getDefaultFont();
};

#include "../shaders/Shader.h"
static nafy::shader_t (*getDefaultTextShader)() = []() -> nafy::shader_t {
    return nafy::getContext()->getDefaultSpriteShader();
};

#endif
