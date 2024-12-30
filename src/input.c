/*
 * FILE: input.c
 * TITLE: Input handling
 * AUTHOR: Simas Bradaitis <simasbra@proton.me>
 * VERSION: 0.1.0
 * DESCRIPTION: Input handling inplementation
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
		if (i_handle_received_key(monitor, value)) {
			pthread_cond_signal(&(monitor->input_received));
		}
		pthread_mutex_unlock(&(monitor->mutex));
	}
}

int i_handle_received_key(monitor *const monitor, const int value)
{
	switch (value) {
	case (int)'q':
		monitor->signal = SIGNAL_GAME_EXIT;
		return 1;
	case KEY_UP:
	case (int)'w':
		monitor->signal = SIGNAL_MOVE_UP;
		return 1;
	case KEY_DOWN:
	case (int)'r':
		monitor->signal = SIGNAL_MOVE_DOWN;
		return 1;
	case KEY_RIGHT:
	case (int)'s':
		monitor->signal = SIGNAL_MOVE_RIGHT;
		return 1;
	case KEY_LEFT:
	case (int)'a':
		monitor->signal = SIGNAL_MOVE_LEFT;
		return 1;
	case -1:
		monitor->signal = SIGNAL_MOVE_PREVIOUS;
		return 1;
	default:
		monitor->signal = SIGNAL_EMPTY;
		return 0;
	}
}
