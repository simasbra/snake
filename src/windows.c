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

#include "windows.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

struct windows *w_malloc(void)
{
	windows *windows = (struct windows *)malloc(sizeof(struct windows));
	if (!windows) {
		perror("ERROR: windows allocation failed:\n");
	}
	return windows;
}

void w_initialize(struct windows *const windows)
{
	int y_max, x_max;
	getmaxyx(stdscr, y_max, x_max);
	windows->game = newwin(y_max - 1, x_max, 0, 0);
	windows->status = newwin(1, x_max, y_max - 1, 0);
	refresh();
	wprintw(windows->status, "Press q to exit or any other key to play\n");
	wrefresh(windows->status);
	box(windows->game, 0, 0);
	wrefresh(windows->game);
}

void w_free(struct windows **windows)
{
	if (!windows || !*windows) {
		return;
	}
	free(*windows);
	*windows = NULL;
}

void w_ncurses_initialize(void)
{
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, 1);
	curs_set(0);
	halfdelay(1);
	if (has_colors()) {
		start_color();
		init_pair(1, COLOR_GREEN, COLOR_BLACK);
		init_pair(2, COLOR_RED, COLOR_BLACK);
	} else {
		fprintf(stderr, "ERROR: Terminal does not support colors\n");
		exit(EXIT_FAILURE);
	}
}

void w_ncurses_finalize(void)
{
	curs_set(1);
	endwin();
}

void w_display(windows *const windows, monitor *const monitor, snake *const snake)
{
	if (!windows || !monitor || !snake) {
		return;
	}
	short exit_received = 0;
	while (!exit_received) {
		pthread_mutex_lock(&(monitor->mutex));
		while (monitor->signal_windows == SIGNAL_WINDOWS_EMPTY) {
			pthread_cond_wait(&(monitor->conditional), &(monitor->mutex));
		}
		if (w_handle_signal(windows, monitor, snake)) {
			pthread_mutex_unlock(&(monitor->mutex));
			return;
		}
		pthread_mutex_unlock(&(monitor->mutex));
	}
}

short w_handle_signal(windows *const windows, monitor *const monitor, snake *const snake)
{
	if (!windows || !monitor || !snake) {
		return 0;
	}
	switch (monitor->signal_windows) {
	case SIGNAL_WINDOWS_GAME_EXIT:
		return 1;
	case SIGNAL_WINDOWS_SNAKE_AND_FOOD_REFRESH:
		w_snake_display_food(windows, snake);
	case SIGNAL_WINDOWS_SNAKE_REFRESH:
		w_snake_display_snake(windows, snake);
		if (snake->tail.y != -1 && snake->tail.x != -1) {
			w_snake_clear_tail(windows, snake);
			snake->tail.x = -1;
			snake->tail.y = -1;
		}
		wrefresh(windows->game);
		break;
	default:
		break;
	}
	monitor->signal_windows = SIGNAL_WINDOWS_EMPTY;
	return 0;
}

void w_snake_display_snake(windows *const windows, const snake *const snake)
{
	if (!windows || !snake) {
		return;
	}
	wattron(windows->game, COLOR_PAIR(1));
	mvwaddch(windows->game, snake->head.y, snake->head.x, ACS_BLOCK);
	wattroff(windows->game, COLOR_PAIR(1));
}

void w_snake_display_food(windows *const windows, const snake *const snake)
{
	if (!windows || !snake) {
		return;
	}
	if (snake->food.x == -1 && snake->food.y == -1) {
		return;
	}
	wattron(windows->game, COLOR_PAIR(2));
	mvwaddch(windows->game, snake->food.y, snake->food.x, '*');
	wattroff(windows->game, COLOR_PAIR(2));
}

void w_snake_clear_tail(windows *const windows, const snake *const snake)
{
	if (!windows || !snake) {
		return;
	}
	mvwaddch(windows->game, snake->tail.y, snake->tail.x, ' ');
}
