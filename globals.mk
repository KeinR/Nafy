OBJS_EXT = stb_image.o stb_truetype.o stb_vorbis.o stb_image_write.o glad.o
OBJS_TEXT = text.Text.o text.TextCrawl.o text.Font.o text.FontFactory.o
OBJS := context.o TextLibrary.o TextRec.o View.o Shader.o Node.o Rectangle.o Button.o Color.o ShaderFactory.o env.o error.o story.o $(OBJS_TEXT)
CFLAGS := -Wall `pkg-config --cflags freetype2`
CC = g++
INCLUDE = -Iextern
LIBS = -lglfw3dll -lfreetype.dll -lstb

MK := $(CC) $(CFLAGS) $(INCLUDE)
