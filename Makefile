OBJS_EXT = stb_image.o stb_truetype.o stb_vorbis.o stb_image_write.o glad.o
OBJS = context.o env.o error.o story.o Text.o Font.o shaders.o
# OBJS = env.o
CFLAGS := -Wall `pkg-config --cflags freetype2`
CC = g++
INCLUDE = -Iextern
LIBS = libglfw3dll.a libfreetype.dll.a

MK := $(CC) $(CFLAGS) $(INCLUDE)


# TEMP

current:
	rm -f test.exe
	make test
	@printf "Size: "
	@stat --printf="%s" test.exe

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
depconfig.mk: src/*
	dep src

$(OBJS):
	$(MK) -c $< -o $@

clean:
	rm -f $(OBJS) $(OBJS_EXT) main.o
