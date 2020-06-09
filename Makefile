# SHELL := /usr/bin/env bash
.SUFFIXES:

include globals.mk
DEV_OUT = ./build-dev.exe
# REL_OUT = ./nafy.a

MAIN = bin/main.o

DEV_BUILD := $(MK) -o $(DEV_OUT) $(MAIN) $(objects) $(LIBS)
DB_BUILD := $(DEV_BUILD)
# REL_BUILD := $(MK) -o $(DEV_OUT) main.o $(DEPS) $(LIBS) -02 -s
# AR = ar crf $(REL_OUT) header.o $(DEPS) $(LIBS)

build-dev: $(MAIN) $(objects)
	$(DEV_BUILD) -g

build-debug: $(MAIN) $(objects)
	$(DB_BUILD) -g

# build-release: $(DEPS)
# 	$(REL_BUILD)

run: build-dev
	@./build-dev.exe

debug: build-debug
	gdb ./build-dev.exe -ex run

# For dev
$(MAIN): main.cpp src/**
	$(MK) -c $< -o $@

$(EXT_OBJ):
	$(MK) -c $< -o $@

$(OBJ):
	$(MK) -c $< -o $@

include depconfig.mk
depconfig.mk: $(shell ./depdep)
	./confdeps

globals.mk:
	@echo "ERROR: globals.mk not found"
	@exit 1

clean:
	rm -f $(objects) $(MAIN)
