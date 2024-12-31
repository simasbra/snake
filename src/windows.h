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

#ifndef __WINDOWS_H__
#define __WINDOWS_H__

#include "snake.h"
#include <ncurses.h>

typedef struct windows {
	WINDOW *game;
	WINDOW *status;
	WINDOW *menu;
} windows;

/*
 * Creates new windows object
 * \RETURNS: pointer to the newly created windows object
 */
struct windows *w_malloc(void);

/*
 * Initializes snake with default values
 */
void w_initialize(struct windows *const windows);

/*
 * Frees the given windows object
 */
void w_free(struct windows **windows);

/*
 * Initializes ncurses
 */
void w_ncurses_initialize(void);

/*
 * Finalizes ncurses
 */
void w_ncurses_finalize(void);

/*
 * Handles windows displaying
 */
void w_display(windows *const windows, monitor *const monitor, const snake *const snake);

short w_handle_signal(windows *const windows, monitor *const monitor, const snake *const snake);

/*
 * Displays snake on the game window
 */
void w_snake_display_snake(windows *const windows, const snake *const snake);

/*
 * Displays snakes food on the game window
 */
void w_snake_display_food(windows *const windows, const snake *const snake);

/*
 * Clears snakes tail to whitespace in the game window
 */
void w_snake_clear_tail(windows *const windows, const snake *const snake);

#endif
