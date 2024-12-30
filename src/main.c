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

#include "monitor.h"
#include "snake.h"
#include "threads.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Initializes ncurses
 */
void ncurses_initialize(void);

/*
 * Finalizes ncurses
 */
void ncurses_finalize(void);

int main(void)
{
	ncurses_initialize();

	int y_max, x_max;
	getmaxyx(stdscr, y_max, x_max);
	WINDOW *game_window = newwin(y_max - 1, x_max, 0, 0);
	WINDOW *status_window = newwin(1, x_max, y_max - 1, 0);
	refresh();
	wprintw(status_window, "Press q to exit or any other key to play\n");
	wrefresh(status_window);
	box(game_window, 0, 0);
	wrefresh(game_window);

	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	srand(time.tv_nsec);

	snake *snake = s_malloc();
	if (!snake) {
		goto finalize_ncurses;
	}
	s_initialize(snake, game_window);

	monitor *monitor = m_malloc();
	if (!monitor) {
		goto finalize_snake;
	}
	m_initialize(monitor);

	pthread_t threads[THREAD_TYPE_COUNT];
	t_initialize_threads(threads, monitor, snake);
	t_finalize_threads(threads);

	m_free(&monitor);
finalize_snake:
	s_free(&snake);
finalize_ncurses:
	ncurses_finalize();
	return 0;
}

void ncurses_initialize(void)
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
void ncurses_finalize(void)
{
	curs_set(1);
	endwin();
}
