/*
 * FILE: snake.c
 * TITLE: Snake logic handling
 * AUTHOR: Simas Bradaitis <simasbra@proton.me>
 * VERSION: 0.1.0
 * DESCRIPTION: Snake movement, death and other logic handling
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

#include "snake.h"
#include <stdlib.h>
#include <stdio.h>

Snake *s_malloc(WINDOW *const game_window)
{
	if (!game_window) {
		fprintf(stderr, "ERROR: game_window is NULL.\n");
		return NULL;
	}
	struct Snake *snake = malloc(sizeof(struct Snake));
	if (!snake) {
		perror("ERROR: Snake malloc failed\n");
		return NULL;
	}
	snake->body = dll_malloc();
	if (!snake->body) {
		free(snake);
		return NULL;
	}
	int y_win, x_win;
	getmaxyx(game_window, y_win, x_win);
	snake->window = game_window;
	snake->length = 1;
	snake->x_head = x_win / 2;
	snake->y_head = y_win / 2;
	snake->x_max = x_win;
	snake->y_max = y_win;

	return snake;
}

void s_free(Snake **snake)
{
	if (!snake) {
		fprintf(stderr, "ERROR: snake is NULL.\n");
		return;
	}

	double_linked_list *list = (*snake)->body;
	dll_free(&list);
	(*snake)->body = NULL;
	free(*snake);
	*snake = NULL;
}

void s_handle_move(Snake *const snake, Monitor *const input)
{
	while (1) {
		pthread_mutex_lock(&input->mutex);
		while (input->signal_type != SIGNAL_EMPTY) {
			pthread_cond_wait(&input->input_received, &input->mutex);
		}
		if (input->signal_type == SIGNAL_GAME_EXIT) {
			break;
		}
		s_handle_signal(snake, input);
		input->signal_type = SIGNAL_EMPTY;
		pthread_mutex_unlock(&input->mutex);
	}
}

void s_handle_signal(Snake *const snake, Monitor *const input)
{
	switch (input->signal_type) {
	case SIGNAL_MOVE_UP:
		s_move_up(snake);
		break;
	case SIGNAL_MOVE_DOWN:
		s_move_down(snake);
		break;
	case SIGNAL_MOVE_RIGHT:
		s_move_right(snake);
		break;
	case SIGNAL_MOVE_LEFT:
		s_move_left(snake);
		break;
	default:
		break;
	}
}

void s_move_up(Snake *const snake)
{
	if (s_check_new_location(snake, snake->x_head, snake->y_head - 1)) {
		return;
	}
	snake->y_head--;
}

void s_move_down(Snake *const snake)
{
	if (s_check_new_location(snake, snake->x_head, snake->y_head + 1)) {
		return;
	}
	snake->y_head++;
}

void s_move_right(Snake *const snake)
{
	if (s_check_new_location(snake, snake->x_head - 1, snake->y_head)) {
		return;
	}
	snake->x_head--;
}

void s_move_left(Snake *const snake)
{
	if (s_check_new_location(snake, snake->x_head + 1, snake->y_head)) {
		return;
	}
	snake->x_head++;
}

int s_check_new_location(const Snake *const snake, int x, int y)
{
	if (x < 1 || y > snake->x_max - 1) {
		return 1;
	}
	if (y < 1 || y > snake->y_max - 1) {
		return 1;
	}
	return 0;
}

void s_display(const Snake *const snake)
{
	mvwaddch(snake->window, snake->y_head, snake->x_head, L'■');
}
