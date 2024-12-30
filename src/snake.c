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
#include "double_linked_list.h"
#include "monitor.h"
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>

snake *s_malloc(WINDOW *const game_window)
{
	if (!game_window) {
		fprintf(stderr, "ERROR: game_window is NULL.\n");
		return NULL;
	}
	struct snake *snake = malloc(sizeof(struct snake));
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
	s_coordinates head = { x_win / 2, y_win / 2 };
	s_coordinates max = { x_win, y_win };
	s_coordinates food = { -1, -1 };

	snake->window = game_window;
	snake->length = 1;
	snake->head = head;
	snake->max = max;
	snake->food = food;
	snake->last_move = SNAKE_MOVE_RIGHT;

	dll_push_beginning(snake->body, &head, sizeof(struct s_coordinates));
	return snake;
}

void s_free(snake **snake)
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

void s_handle_move(snake *const snake, monitor *const monitor)
{
	if (!snake || !monitor) {
		return;
	}
	s_generate_food(snake);
	int exit_received = 0;
	while (!exit_received) {
		s_display(snake);
		pthread_mutex_lock(&(monitor->mutex));
		while (monitor->signal == SIGNAL_EMPTY) {
			pthread_cond_wait(&(monitor->input_received), &(monitor->mutex));
		}
		if (monitor->signal == SIGNAL_GAME_EXIT) {
			exit_received = 1;
		}
		s_handle_signal(snake, monitor);
		pthread_mutex_unlock(&(monitor->mutex));
		s_handle_food(snake);
	}
}

void s_handle_signal(snake *const snake, monitor *const monitor)
{
	if (!snake || !monitor) {
		return;
	}
	switch (monitor->signal) {
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
	case SIGNAL_MOVE_PREVIOUS:
		s_move_previous(snake);
		break;
	default:
		break;
	}
	monitor->signal = SIGNAL_EMPTY;
}

void s_move_up(snake *const snake)
{
	if (!snake) {
		return;
	}
	if (!s_check_new_location(snake, snake->head.x, snake->head.y - 1)) {
		return;
	}
	snake->head.y--;
	snake->last_move = SNAKE_MOVE_UP;
}

void s_move_down(snake *const snake)
{
	if (!snake) {
		return;
	}
	if (!s_check_new_location(snake, snake->head.x, snake->head.y + 1)) {
		return;
	}
	snake->head.y++;
	snake->last_move = SNAKE_MOVE_DOWN;
}

void s_move_right(snake *const snake)
{
	if (!snake) {
		return;
	}
	if (!s_check_new_location(snake, snake->head.x + 1, snake->head.y)) {
		return;
	}
	snake->head.x++;
	snake->last_move = SNAKE_MOVE_RIGHT;
}

void s_move_left(snake *const snake)
{
	if (!snake) {
		return;
	}
	if (!s_check_new_location(snake, snake->head.x - 1, snake->head.y)) {
		return;
	}
	snake->head.x--;
	snake->last_move = SNAKE_MOVE_LEFT;
}

void s_move_previous(snake *const snake)
{
	if (!snake) {
		return;
	}
	switch (snake->last_move) {
	case SNAKE_MOVE_UP:
		s_move_up(snake);
		break;
	case SNAKE_MOVE_DOWN:
		s_move_down(snake);
		break;
	case SNAKE_MOVE_RIGHT:
		s_move_right(snake);
		break;
	case SNAKE_MOVE_LEFT:
		s_move_left(snake);
		break;
	default:
		break;
	}
}

int s_check_new_location(const snake *const snake, int x, int y)
{
	if (!snake) {
		return 0;
	}
	if (x < 1) {
		return 0;
	} else if (y < 1) {
		return 0;
	} else if (x > snake->max.x - 2) {
		return 0;
	} else if (y > snake->max.y - 2) {
		return 0;
	}
	return 1;
}

void s_display(const snake *const snake)
{
	if (!snake) {
		return;
	}
	mvwaddch(snake->window, snake->head.y, snake->head.x, '@');
	wrefresh(snake->window);
}

void s_clear_tail(const snake *const snake)
{
	s_coordinates *tail = (struct s_coordinates *)dll_get_index(snake->body, snake->length);
	mvwaddch(snake->window, tail->y, tail->x, ' ');
}

void s_generate_food(snake *const snake)
{
	if (!snake) {
		return;
	}
	struct timeval time;
	gettimeofday(&time, NULL);
	srand(time.tv_usec);
	snake->food.x = rand() % (snake->max.x - 2) + 1;
	snake->food.y = rand() % (snake->max.y - 2) + 1;
	mvwaddch(snake->window, snake->food.y, snake->food.x, '*');
}

int s_check_food(const snake *const snake)
{
	if (!snake) {
		return 0;
	}
	if (snake->head.x == snake->food.x && snake->head.y == snake->food.y) {
		return 1;
	}
	return 0;
}

void s_handle_food(snake *const snake)
{
	if (!snake) {
		return;
	}

	dll_push_beginning(snake->body, &snake->head, sizeof(struct s_coordinates));
	if (s_check_food(snake)) {
		snake->length++;
		s_generate_food(snake);
	} else {
		s_clear_tail(snake);
		s_coordinates *tail = (s_coordinates *)dll_pop_end(snake->body);
		free(tail);
	}
}
