# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wpedantic -Wconversion -std=c99
CFLAGS += -fsanitize=address

# Libraries
LIBS = -lncurses -lpthread

# Subdirectory for object files
O = obj

# Subdirectory of source files
S = src

# Subdirectory for binary files
B = bin

# Target file
TARGET = $(B)/snake

ifeq ($(OS),Windows_NT)
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CFLAGS += -std=gnu99
    endif
    ifeq ($(UNAME_S),Darwin)
	CC = clang
    endif
endif

# Object files
OBJS = $(O)/snake.o \
	$(O)/input.o \
	$(O)/monitor.o \
	$(O)/threads.o \
	$(O)/windows.o \
	$(O)/circular_dynamic_queue.o

# Rules
.PHONY: all snake outdir clean

all: outdir snake

snake: $(OBJS) $(O)/main.o
	$(CC) $(CFLAGS) $(OBJS) $(O)/main.o -o $(TARGET) $(LIBS)

$(O)/%.o: $(S)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

outdir:
	mkdir -p $(O) $(B)

clean:
	rm -rf $(O) $(B)
