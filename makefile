# Compiler and flags
CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic -std=c99
# CFLAGS += -fsanitize=address

# Libraries
LIBS = -lncurses -lpthread

# Subdirectory for object files
O = objs

# Subdirectory of source files
S = src

# Target file
TARGET = snake

ifeq ($(OS),Windows_NT)
    CFLAGS += -D WIN32
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
	CC = gcc
        CFLAGS += -D LINUX -std=gnu99
    endif
    ifeq ($(UNAME_S),Darwin)
	CC = clang
        CFLAGS += -D OSX
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
all: snake

snake: $(OBJS) $(O)/main.o
	$(CC) $(CFLAGS) $(OBJS) $(O)/main.o -o $(TARGET) $(LIBS)

$(O)/%.o: $(S)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)
	rm -rf snake.out
