OBJS_EXT = stb_image.o stb_truetype.o stb_vorbis.o stb_image_write.o glad.o
OBJS_TEXT = text.Text.o text.TextCrawl.o text.Font.o text.Face.o
OBJS := context.o TextLibrary.o View.o Shader.o Node.o Rectangle.o Button.o Color.o ShaderFactory.o env.o error.o story.o $(OBJS_TEXT)
CFLAGS := -Wall `pkg-config --cflags freetype2`
CC = g++
INCLUDE = -Iextern
LIBS = -lglfw3dll -lfreetype.dll -lstb

MK := $(CC) $(CFLAGS) $(INCLUDE)


# TEMP

run: test
	@./test.exe

test: main.o $(OBJS) $(OBJS_EXT)
	$(MK) -o $@ main.o $(OBJS) $(OBJS_EXT) $(LIBS)

main.o: main.cpp src/context.h src/story.h
	$(MK) -c $< -o $@

# END TEMP

stb_image.o: extern/stb/stb_image.c extern/stb/stb_image.h
stb_truetype.o: extern/stb/stb_truetype.c extern/stb/stb_truetype.h
stb_vorbis.o: extern/stb/stb_vorbis.c extern/stb/stb_vorbis.h
stb_image_write.o: extern/stb/stb_image_write.c extern/stb/stb_image_write.h
glad.o: extern/glad/glad.c extern/glad/glad.h

$(OBJS_EXT):
	$(MK) -c $< -o $@

include depconfig.mk
depconfig.mk: src/* src/text/*
	dep ./src

$(OBJS):
	$(MK) -c $< -o $@

clean:
	rm -f $(OBJS) $(OBJS_EXT) main.o
