# Compiler and flags
CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic -std=c99
# CFLAGS += -fsanitize=address

# Libraries
LIBS = -lncurses

# Subdirectory for object files
O = objs

# Subdirectory of source files
S = src

# Target file
TARGET = snake

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
