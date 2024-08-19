# Makefile
COMPILER = gcc
COMPILER_FLAGS = -o
SOURCE_FILES = src/main.c
TARGET = dist/fetch
PREFIX = /usr/bin

build: $(SOURCE_FILES)
	$(COMPILER) $(COMPILER_FLAGS) $(TARGET) $(SOURCE_FILES)		