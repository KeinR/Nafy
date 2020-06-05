include objects.mk

objects.mk: $(shell ./depdep)
	./confobjs

CFLAGS = -Wall `pkg-config --cflags freetype2`
CC = g++
INCLUDE = -Iextern
LIBS = -lglfw3dll -lfreetype.dll -lstb

MK = $(CC) $(CFLAGS) $(INCLUDE)
