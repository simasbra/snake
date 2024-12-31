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

#include "threads.h"
#include "input.h"
#include "monitor.h"
#include "snake.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void *t_initalize_input(void *args)
{
	struct monitor *monitor = (struct monitor *)args;
	i_handle_input(monitor);
	return NULL;
}

void *t_initalize_snake(void *args)
{
	snake_args *snake_input = (snake_args *)args;
	s_move(snake_input->snake, snake_input->monitor);
	free(snake_input);
	return NULL;
}

void t_initialize_threads(pthread_t *const threads, monitor *const monitor, snake *const snake)
{
	if (!threads || !monitor) {
		return;
	}

	snake_args *snake_input = (snake_args *)malloc(sizeof(snake_args));
	if (!snake_input) {
		return;
	}
	snake_input->monitor = monitor;
	snake_input->snake = snake;

	if (pthread_create(&(threads[THREAD_GAME]), NULL, t_initalize_snake, snake_input) != 0) {
		perror("Game thread create failed:\n");
		free(snake_input);
		return;
	}
	if (pthread_create(&(threads[THREAD_INPUT]), NULL, t_initalize_input, monitor) != 0) {
		perror("Input thread create failed:\n");
		pthread_join(threads[THREAD_GAME], NULL);
		free(snake_input);
		return;
	}
}

void t_finalize_threads(pthread_t *const threads)
{
	if (pthread_join(threads[THREAD_GAME], NULL) != 0) {
		perror("Game thread join failed:\n");
	}
	if (pthread_join(threads[THREAD_INPUT], NULL) != 0) {
		perror("Input thread join failed:\n");
	}
}
