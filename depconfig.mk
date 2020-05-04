context.o: src/context.cpp src/context.h src/error.h
src/context.h: src/glfw.h src/story.h
env.o: src/env.cpp src/env.h src/defs.h src/error.h
src/env.h: src/glfw.h
error.o: src/error.cpp src/error.h
story.o: src/story.cpp src/story.h src/error.h
