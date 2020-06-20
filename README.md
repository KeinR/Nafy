# NAFY

That used to stand for something, but I've since forgotten what it was.

# About

Nafy is an attempt to create a visual novel framework in OpenGL... It's not just OpenGL though
(see `libraries`).

## Features

- Text (`src/text/Text`)
- Text crawls (`src/text/TextCrawl`)
- "generic" shaders that allow for user defined types (`src/shaders`)
- Abstracted OpenGL rendering (`src/render`)
- Mafs (`src/math`)
    - I literally made all those classes just for `src/gui/Button`... It's a long story.
- Little gui widgets (`src/gui`)
    - Button
    - Image
    - Rectangle
    - TextRec
- Game events for when the VN is rolling (`src/game`)
    - BasicEvent
    - ButtonPrompt
    - FuncEvent
    - TextString
- Game logic (`src/game/Scene` && `src/game/sceneEvent`)
- Sound playback (`src/audio`)
    - Streaming (`AudioStream`)

etc. etc.

## Size

Just to give you an idea, [loc](https://github.com/cgag/loc) reports:
```
--------------------------------------------------------------------------------
 Language             Files        Lines        Blank      Comment         Code
--------------------------------------------------------------------------------
 C++                     41         5583          725          658         4200
 C/C++ Header            47         3429          469         1404         1556
--------------------------------------------------------------------------------
 Total                   88         9012         1194         2062         5756
--------------------------------------------------------------------------------
```
Yes, I know lines-of-code is a terrible metric... However, I can't easially think of
any other way of describing the size of what's in `./src/`.

## The Future

*Haitus.*

I've commented the sourcecode extensively, however I think that I'll take a bit of a break from Nafy.
I worked on it for over a month (almost two!), and I'm quite frankly tired of it. I have a few game
ideas I've been working on, and I plan to use what I learned and some of the code from here to help
there.

If anyone wants to open a PR, I'd be down. I've committed a TODO that has vague descriptions of
problems I encountered / features.

# Building

There's a weird demo in `main.cpp`.

Need:
- Libraries (see `Libraries`)
    - Add archives to linker path and include dirs to the include path
    - Note that in `globals.mk`, there's `pkg-config --cflags freetype2`.
        if you don't have a linux-like system and/or you didn't "install" freetype,
        then you have to remove that and replace it with the include dir of your freetype
        installation.
- GNU GCC (or something else if you can get it to work)
- GNU make
- OpenGL 3.3+ (shouldn't be an issue, but I thought I'd mention it)

The building is simple enough. Enter the root dir in terminal and type `make build-dev`,
or `make run` if you want to run it right away.

That builds the demo. To build as an archive, just add another make rule with the `objects` macro.
(PS: `depconfig.mk` and `objects.mk` were indeed programmically generated)

# Libraries

- glfw (DLL) https://www.glfw.org/
- OpenAL-Soft (DLL) https://openal-soft.org/
- Freetype (DLL) https://www.freetype.org/download.html
- OpenGL Mathematics (header only) https://glm.g-truc.net/0.9.9/index.html
- GLAD (static) https://github.com/Dav1dde/glad
- stb (Sean Barrett) https://github.com/nothings/stb
    - stb_image.h
    - stb_vorbis.h

Of the above, the ones that aren't included (the ones you'll have to download/build, and add
to linker and/or include path) are:

- glfw (DLL) https://www.glfw.org/
- OpenAL-Soft (DLL) https://openal-soft.org/
- Freetype (DLL) https://www.freetype.org/download.html
- OpenGL Mathematics (header only) https://glm.g-truc.net/0.9.9/index.html
