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

#ifndef __THREADS_H__
#define __THREADS_H__

#include "snake.h"
#include "monitor.h"
#include "windows.h"
#include <pthread.h>

/*
 * Thread types
 */
typedef enum w_thread { THREAD_INPUT, THREAD_GAME, THREAD_WINDOWS, THREAD_TYPE_COUNT } w_thread;

/*
 * Arguments for snake thread
 */
typedef struct snake_args {
	monitor *monitor;
	snake *snake;
} snake_args;

/*
 * Arguments for windows thread
 */
typedef struct windows_args {
	monitor *monitor;
	snake *snake;
	windows *windows;
} windows_args;

/*
 * Initializes input handling thread
 */
void *t_initialize_input(void *args);

/*
 * Initializes snake movement handling thread
 */
void *t_initialize_snake(void *args);

/*
 * Initializes windows displaying thread;
 */
void *t_initialize_windows(void *args);

/*
 * Initializes threads by giving jobs to them
 */
void t_initialize_threads(pthread_t *const threads, monitor *const monitor, snake *const snake,
			  windows *const windows);

/*
 * Finalizes threads by joining them
 */
void t_finalize_threads(pthread_t *const threads);

#endif
