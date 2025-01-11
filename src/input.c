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
		i_handle_exit(monitor);
		return 1;
	case KEY_UP:
		i_handle_move(monitor, SNAKE_MOVE_UP, SNAKE_MOVE_DOWN);
		return 1;
	case KEY_DOWN:
		i_handle_move(monitor, SNAKE_MOVE_DOWN, SNAKE_MOVE_UP);
		return 1;
	case KEY_RIGHT:
		i_handle_move(monitor, SNAKE_MOVE_RIGHT, SNAKE_MOVE_LEFT);
		return 1;
	case KEY_LEFT:
		i_handle_move(monitor, SNAKE_MOVE_LEFT, SNAKE_MOVE_RIGHT);
		return 1;
	case -1:
		return 0;
	default:
		return 0;
	}
}

void i_handle_exit(monitor *const monitor)
{
	if (!monitor) {
		return;
	}
	monitor->signal_snake = SIGNAL_SNAKE_GAME_EXIT;
	monitor->signal_windows = SIGNAL_WINDOWS_GAME_EXIT;
	pthread_cond_signal(&(monitor->conditional));
}

void i_handle_move(monitor *const monitor, const enum m_snake_move next_move,
		   const enum m_snake_move opposite_direction)
{
	if (!monitor || next_move == SNAKE_MOVE_EMPTY || opposite_direction == SNAKE_MOVE_EMPTY) {
		return;
	}
	if (monitor->move_previous == opposite_direction) {
		return;
	}

	if (monitor->move_next[0] == SNAKE_MOVE_EMPTY && monitor->move_next[0] != next_move) {
		monitor->signal_snake = SIGNAL_SNAKE_MOVE;
		monitor->move_next[0] = next_move;
	} else if (monitor->move_next[1] == SNAKE_MOVE_EMPTY
		   && monitor->move_next[0] != next_move) {
		monitor->signal_snake = SIGNAL_SNAKE_MOVE;
		monitor->move_next[1] = next_move;
	}
}
