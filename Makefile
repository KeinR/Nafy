EXT_OBJ = bin/extern.glad.glad.o bin/extern.stb.stb_image.o bin/extern.stb.stb_image_write.o bin/extern.stb.stb_vorbis.o 
OBJ = bin/audio.AudioContext.o bin/audio.Device.o bin/audio.SoundBuffer.o bin/audio.SoundData.o bin/audio.Speaker.o bin/core.context.o bin/core.error.o bin/core.TextLibrary.o bin/core.Window.o bin/env.env.o bin/game.BasicEvent.o bin/game.ButtonPrompt.o bin/game.Scene.o bin/game.sceneEvent.o bin/game.TextString.o bin/gui.Button.o bin/gui.Color.o bin/gui.Image.o bin/gui.Node.o bin/gui.Rectangle.o bin/gui.TextRec.o bin/gui.View.o bin/render.Buffer.o bin/render.Model.o bin/render.Texture.o bin/shaders.Shader.o bin/shaders.ShaderFactory.o bin/text.Font.o bin/text.FontFactory.o bin/text.Text.o bin/text.TextCrawl.o 
objects := $(OBJ) $(EXT_OBJ)

CFLAGS = -Wall `pkg-config --cflags freetype2` -s
CC = g++
INCLUDE = -Iextern
LIBS = -lglfw3dll -lfreetype.dll -lOpenAL32.dll

MK = $(CC) $(CFLAGS) $(INCLUDE)
OUT = ./nafy.exe

MAIN = bin/main.o

.SUFFIXES:

release: bin $(MAIN) $(objects)
	$(MK) -o $(OUT) $(MAIN) $(objects) $(LIBS)

run: release
	@$(OUT)

include depconfig.mk

# For dev
$(MAIN): main.cpp src/**
	$(MK) -c $< -o $@

$(EXT_OBJ):
	$(MK) -c $< -o $@

$(OBJ):
	$(MK) -c $< -o $@

bin:
	mkdir -p bin

depconfig.mk:
	@echo "ERROR: depconfig.mk not found"
	@exit 1

clean:
	rm -f $(objects) $(MAIN)

clean-all: clean
	rm -f $(OUT)
