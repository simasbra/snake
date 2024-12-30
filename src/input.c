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

#include "input.h"
#include "monitor.h"
#include <ncurses.h>

void i_handle_input(monitor *const monitor)
{
	int exit_received = 0;
	while (!exit_received) {
		int value = getch();
		if (value == (int)'q') {
			exit_received = 1;
		}
		pthread_mutex_lock(&(monitor->mutex));
		i_handle_received_key(monitor, value);
		pthread_mutex_unlock(&(monitor->mutex));
	}
}

short i_handle_received_key(monitor *const monitor, const int value)
{
	switch (value) {
	case (int)'q':
		monitor->signal = SIGNAL_GAME_EXIT;
		return 1;
	case KEY_UP:
	case (int)'w':
		i_handle_key_up(monitor);
		return 1;
	case KEY_DOWN:
	case (int)'r':
		i_handle_key_down(monitor);
		return 1;
	case KEY_RIGHT:
	case (int)'s':
		i_handle_key_right(monitor);
		return 1;
	case KEY_LEFT:
	case (int)'a':
		i_handle_key_left(monitor);
		return 1;
	case -1:
		return 0;
	default:
		monitor->signal = SIGNAL_EMPTY;
		return 0;
	}
}

void i_handle_key_up(monitor *const monitor)
{
	if (!monitor) {
		return;
	}
	switch (monitor->move_previous) {
	case SNAKE_MOVE_DOWN:
	case SNAKE_MOVE_UP:
		return;
	default:
		monitor->move_next = SNAKE_MOVE_UP;
	}
}

void i_handle_key_down(monitor *const monitor)
{
	if (!monitor) {
		return;
	}
	switch (monitor->move_previous) {
	case SNAKE_MOVE_UP:
	case SNAKE_MOVE_DOWN:
		return;
	default:
		monitor->move_next = SNAKE_MOVE_DOWN;
	}
}

void i_handle_key_right(monitor *const monitor)
{
	if (!monitor) {
		return;
	}
	switch (monitor->move_previous) {
	case SNAKE_MOVE_RIGHT:
	case SNAKE_MOVE_LEFT:
		return;
	default:
		monitor->move_next = SNAKE_MOVE_RIGHT;
	}
}

void i_handle_key_left(monitor *const monitor)
{
	if (!monitor) {
		return;
	}
	switch (monitor->move_previous) {
	case SNAKE_MOVE_LEFT:
	case SNAKE_MOVE_RIGHT:
		return;
	default:
		monitor->move_next = SNAKE_MOVE_LEFT;
	}
}
