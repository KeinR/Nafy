include globals.mk
DEV_OUT = ./test.exe
# REL_OUT = ./nafy.a

DEPS := $(OBJS) $(OBJS_EXT)
DEV_BUILD := $(MK) -o $(DEV_OUT) main.o $(DEPS) $(LIBS)
DB_BUILD := $(DEV_BUILD)
# REL_BUILD := $(MK) -o $(DEV_OUT) main.o $(DEPS) $(LIBS) -02 -s
# AR = ar crf $(REL_OUT) header.o $(DEPS) $(LIBS)

build-dev: main.o $(DEPS)
	$(DEV_BUILD)

build-debug: main.o $(DEPS)
	$(DB_BUILD) -g

# build-release: $(DEPS)
# 	$(REL_BUILD)

run: build-dev
	@./test.exe

debug: build-debug
	gdb ./test.exe -ex run

# For dev
main.o: main.cpp src/context.h src/story.h
	$(MK) -c $< -o $@


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

globals.mk:
	@echo "ERROR: globals.mk not found"
	@exit 1

$(OBJS):
	$(MK) -c $< -o $@

clean:
	rm -f $(OBJS) $(OBJS_EXT) $(DEV_OUT) main.o
