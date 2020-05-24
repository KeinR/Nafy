#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

// Should only be included by *.cpp files so as to prevent cluttering the defs for users

#define MSSA 5
#define TEXT_ESC '\033'
#define TEXT_SHIFT_OUT '\016'
#define TEXT_SHIFT_IN '\017'

#define SHADER_MODEL_NAME "model"
#define SHADER_SPRITE_SAMPLER_NAME "Tex"
#define SHADER_TEXT_SAMPLER_NAME "Tex"

// Paths, MUST have a slash at end to allow for proper concatenation
#define PATH_FONTS "resources/fonts/"
#define PATH_SHADERS "resources/shaders/"

#endif
