/*
 * FILE: snake.h
 * TITLE: Snake logic handling
 * AUTHOR: Simas Bradaitis <simasbra@proton.me>
 * VERSION: 0.1.0
 * DESCRIPTION: Snake movement, death and other logic handling header file
 *
 * Copyright (c) 2024 Simas Bradaitis
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "monitor.h"
#include "double_linked_list.h"
#include <ncurses.h>
#include <stddef.h>

/*
 * Snake move types
 */
typedef enum s_snake_move {
	SNAKE_MOVE_UP,
	SNAKE_MOVE_DOWN,
	SNAKE_MOVE_RIGHT,
	SNAKE_MOVE_LEFT
} s_snake_move;

typedef struct s_coordinates {
	int x;
	int y;
} s_coordinates;

/*
 * Struct for storing snake information
 */
typedef struct snake {
	struct s_coordinates head;
	struct s_coordinates max;
	struct s_coordinates food;
	size_t length;
	WINDOW *window;
	enum s_snake_move last_move;
	struct double_linked_list *body;
} snake;

/*
 * Creates new snake object
 * \RETURNS: pointer to the newly created snake object
 */
snake *s_malloc(WINDOW *const game_window);

/*
 * Frees the given snake object
 */
void s_free(snake **snake);

/*
 * Handles snake movement
 */
void s_handle_move(snake *const snake, monitor *const monitor);

/*
 * Handles received signal type from input
 */
void s_handle_signal(snake *const snake, monitor *const monitor);

/*
 * Moves snake head up
 */
void s_move_up(snake *const snake);

/*
 * Moves snake head down
 */
void s_move_down(snake *const snake);

/*
 * Moves snake head to the right
 */
void s_move_right(snake *const snake);

/*
 * Moves snake head to the left
 */
void s_move_left(snake *const snake);

/*
 * Moves snake head to the direction it's currently facing
 */
void s_move_previous(snake *const snake);

/*
 * Checks if given coordinates are not out of bounds
 * and not inside snake body
 */
int s_check_new_location(const snake *const snake, int x, int y);

/*
 * Displays snake on the game window
 */
void s_display(const snake *const snake);

/*
 * Clears snakes tail to whitespace
 */
void s_clear_tail(const snake *const snake);

/*
 * Generates snake food
 */
void s_generate_food(snake *const snake);

/*
 * Checks if snakes head is on food
 * \RETURNS: 1 if it is on food, 0 if not
 */
int s_check_food(const snake *const snake);

/*
 * Handles checks if snake eating food and handles it
 */
void s_handle_food(snake *const snake);

#endif
