# Compiler and flags
CC = clang
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99
# CFLAGS += -fsanitize=address

# Libraries
LIBS = -lncurses

# Subdirectory for object files
O = objs

# Subdirectory of source files
S = src

# Object files
OBJS = $(O)/snake.o \
	$(O)/double_linked_list.o

# Rules
all: snake

snake: $(OBJS) $(O)/main.o
	$(CC) $(CFLAGS) $(OBJS) $(O)/main.o -o snake.out $(LIBS)

$(O)/%.o: $(S)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf (OBJS)
	rm -rf snake.out
