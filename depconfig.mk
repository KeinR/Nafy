context.o: src/context.cpp src/TextLibrary.h src/context.h src/env.h src/error.h src/glfw.h src/shaders.h src/story.h src/text/Font.h src/text/Text.h src/text/TextCrawl.h src/text/ftype.h
env.o: src/env.cpp src/defs.h src/env.h src/error.h src/glfw.h src/shaders.h
error.o: src/error.cpp src/error.h
shaders.o: src/shaders.cpp src/env.h src/glfw.h src/shaders.h
story.o: src/story.cpp src/TextLibrary.h src/context.h src/error.h src/glfw.h src/shaders.h src/story.h src/text/Font.h src/text/TextCrawl.h src/text/ftype.h
text.Face.o: src/text/Face.cpp src/text/Face.h
text.Font.o: src/text/Font.cpp src/text/Font.h
text.Text.o: src/text/Text.cpp src/text/Text.h src/text/textdefs.h
text.TextCrawl.o: src/text/TextCrawl.cpp src/text/TextCrawl.h src/text/textdefs.h
TextLibrary.o: src/TextLibrary.cpp src/TextLibrary.h src/error.h src/text/Font.h src/text/ftype.h
Window.o: src/Window.cpp src/Window.h src/glfw.h
