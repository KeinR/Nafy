context.o: src/context.cpp src/context.h src/error.h src/env.h src/shaders.h src/Font.h src/Text.h
src/context.h: src/glfw.h src/story.h
env.o: src/env.cpp src/env.h src/defs.h src/error.h src/shaders.h
src/env.h: src/glfw.h
error.o: src/error.cpp src/error.h
Font.o: src/Font.cpp src/Font.h src/env.h src/defs.h
shaders.o: src/shaders.cpp src/shaders.h src/glfw.h src/env.h
story.o: src/story.cpp src/story.h src/context.h src/error.h
Text.o: src/Text.cpp src/Text.h src/glfw.h src/env.h src/defs.h
src/Text.h: src/Font.h
