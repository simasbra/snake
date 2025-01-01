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
#include "snake.h"
#include "windows.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void *t_initialize_input(void *args)
{
	if (!args) {
		return NULL;
	}
	struct monitor *monitor = (struct monitor *)args;
	i_handle_input(monitor);
	return NULL;
}

void *t_initialize_snake(void *args)
{
	if (!args) {
		return NULL;
	}
	struct snake_args *snake_args = (struct snake_args *)args;
	s_move(snake_args->snake, snake_args->monitor);
	free(snake_args);
	return NULL;
}

void *t_initialize_windows(void *args)
{
	if (!args) {
		return NULL;
	}
	windows_args *windows_args = (struct windows_args *)args;
	w_snake_display_head(windows_args->windows, windows_args->snake, COLOR_PAIR_GREEN);
	w_snake_display_food(windows_args->windows, windows_args->snake);
	w_display(windows_args->windows, windows_args->monitor, windows_args->snake);
	free(windows_args);
	return NULL;
}

void t_initialize_threads(pthread_t *const threads, monitor *const monitor, snake *const snake,
			  windows *const windows)
{
	if (!threads || !monitor) {
		return;
	}

	struct snake_args *snake_args = (struct snake_args *)malloc(sizeof(struct snake_args));
	if (!snake_args) {
		return;
	}
	struct windows_args *windows_args
		= (struct windows_args *)malloc(sizeof(struct windows_args));
	if (!windows_args) {
		goto t_free_snake_args;
	}

	snake_args->monitor = monitor;
	snake_args->snake = snake;
	windows_args->monitor = monitor;
	windows_args->snake = snake;
	windows_args->windows = windows;

	if (pthread_create(&(threads[THREAD_GAME]), NULL, t_initialize_snake, snake_args) != 0) {
		perror("Game thread create failed:\n");
		goto t_free_windows_args;
	}
	if (pthread_create(&(threads[THREAD_INPUT]), NULL, t_initialize_input, monitor) != 0) {
		perror("Input thread create failed:\n");
		goto t_join_game_thread;
	}
	if (pthread_create(&(threads[THREAD_WINDOWS]), NULL, t_initialize_windows, windows_args)
	    != 0) {
		perror("Windows thread create failed:\n");
		pthread_join(threads[THREAD_INPUT], NULL);
t_join_game_thread:
		pthread_join(threads[THREAD_GAME], NULL);
t_free_windows_args:
		free(windows_args);
t_free_snake_args:
		free(snake_args);
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
	if (pthread_join(threads[THREAD_WINDOWS], NULL) != 0) {
		perror("Input thread join failed:\n");
	}
}
