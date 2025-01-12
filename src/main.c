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
#include "windows.h"
#include <stdlib.h>

int main(void)
{
	w_ncurses_initialize();

	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	srand((unsigned int)time.tv_nsec);

	windows *windows = w_malloc();
	if (!windows) {
		goto main_finalize_ncurses;
	}
	w_initialize(windows);

	snake *snake = s_malloc();
	if (!snake) {
		goto main_finalize_windows;
	}
	s_initialize(snake, windows->game);

	monitor *monitor = m_malloc();
	if (!monitor) {
		goto main_finalize_snake;
	}
	m_initialize(monitor);

	pthread_t threads[THREAD_TYPE_COUNT];
	t_initialize_threads(threads, monitor, snake, windows);
	t_finalize_threads(threads);

	m_free(&monitor);
main_finalize_snake:
	s_free(&snake);
main_finalize_windows:
	w_free(&windows);
main_finalize_ncurses:
	w_ncurses_finalize();
	return 0;
}
