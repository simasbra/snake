/*
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
#include <time.h>

/*
 * Coordinates
 */
typedef struct s_coordinates {
	int x;
	int y;
} s_coordinates;

/*
 * Struct for storing snake information
 */
typedef struct snake {
	unsigned int score;
	struct s_coordinates head;
	struct s_coordinates tail;
	struct s_coordinates max;
	struct s_coordinates food;
	struct double_linked_list *body;
} snake;

/*
 * Creates new snake object
 * \RETURNS: pointer to the newly created snake object
 */
snake *s_malloc(void);

/*
 * Initializes snake with default values
 */
void s_initialize(snake *const snake, WINDOW *const game_window);

/*
 * Frees the given snake object
 */
void s_free(snake **snake);

/*
 * Controls snake movement
 */
void s_move(snake *const snake, monitor *const monitor);

/*
 * Handles received signal type from input
 * \RETURNS: 1 if exit was signaled, 0 if not
 */
short s_handle_signal(snake *const snake, monitor *const monitor);

/*
 * Handles specified snake move
 * \RETUNRS: 1 if move was valid, 0 if not
 */
void s_handle_move(snake *const snake, monitor *const monitor);

/*
 * Moves snake head up.
 * If snake hits its own body or wall, it will marked as dead
 */
void s_move_up(snake *const snake, monitor *const monitor);

/*
 * Moves snake head down.
 * If snake hits its own body or wall, it will marked as dead
 */
void s_move_down(snake *const snake, monitor *const monitor);

/*
 * Moves snake head to the right.
 * If snake hits its own body or wall, it will marked as dead
 */
void s_move_right(snake *const snake, monitor *const monitor);

/*
 * Moves snake head to the left.
 * If snake hits its own body or wall, it will marked as dead
 */
void s_move_left(snake *const snake, monitor *const monitor);

/*
 * Checks if given coordinates are not out of bounds
 * and not inside snake body
 */
short s_check_new_location(const snake *const snake, int x, int y);

/*
 * Generates snake food
 */
void s_generate_food(snake *const snake);

/*
 * Checks if snakes head is on food
 * \RETURNS: 1 if it is on food, 0 if not
 */
short s_check_food(const snake *const snake);

/*
 * Handles checks if snake eating food and handles it
 * \RETURNS: 1 if snake ate food, 0 if not
 */
short s_handle_food(snake *const snake);

/*
 * Signals windows to update the screen depending on signal type
 */
void s_signal_windows(monitor *const monitor, enum m_signal_windows signal);

#endif
