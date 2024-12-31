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

#include "snake.h"
#include "monitor.h"
#include <stdlib.h>
#include <stdio.h>

snake *s_malloc(void)
{
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
	return snake;
}

void s_initialize(snake *const snake, WINDOW *const game_window)
{
	if (!snake || !game_window) {
		return;
	}
	int y_win, x_win;
	getmaxyx(game_window, y_win, x_win);
	s_coordinates head = { x_win / 2, y_win / 2 };
	s_coordinates max = { x_win, y_win };
	s_coordinates food = { -1, -1 };

	snake->head = head;
	snake->max = max;
	snake->food = food;
	snake->score = 0;

	dll_push_beginning(snake->body, &head, sizeof(struct s_coordinates));
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

void s_move(snake *const snake, monitor *const monitor)
{
	if (!snake || !monitor) {
		return;
	}
	s_generate_food(snake);
	struct timespec sleep_time;
	sleep_time.tv_sec = 0;
	sleep_time.tv_nsec = 1e8;
	pthread_mutex_lock(&(monitor->mutex));
	monitor->signal_windows = SIGNAL_WINDOWS_REFRESH_FOOD;
	pthread_cond_signal(&(monitor->conditional));
	pthread_mutex_unlock(&(monitor->mutex));
	while (1) {
		pthread_mutex_lock(&(monitor->mutex));
		monitor->signal_windows = SIGNAL_WINDOWS_REFRESH_SNAKE;
		pthread_cond_signal(&(monitor->conditional));
		if (s_handle_signal(snake, monitor)) {
			pthread_mutex_unlock(&(monitor->mutex));
			break;
		}
		pthread_mutex_unlock(&(monitor->mutex));
		s_handle_food(snake, monitor);
		nanosleep(&sleep_time, NULL);
	}
}

short s_handle_signal(snake *const snake, monitor *const monitor)
{
	if (!snake || !monitor) {
		return 0;
	}
	if (monitor->signal_snake == SIGNAL_SNAKE_GAME_EXIT) {
		monitor->signal_snake = SIGNAL_SNAKE_EMPTY;
		return 1;
	}

	enum m_snake_move move = SNAKE_MOVE_EMPTY;
	if (monitor->move_next[0] != SNAKE_MOVE_EMPTY) {
		move = monitor->move_next[0];
	} else {
		move = monitor->move_previous;
	}

	if (s_handle_move(snake, move)) {
		monitor->move_next[0] = monitor->move_next[1];
		monitor->move_next[1] = SNAKE_MOVE_EMPTY;
		monitor->move_previous = move;
		snake->score++;
	}
	return 0;
}

short s_handle_move(snake *const snake, enum m_snake_move move)
{
	if (!snake) {
		return 0;
	}
	switch (move) {
	case SNAKE_MOVE_UP:
		s_move_up(snake);
		return 1;
	case SNAKE_MOVE_DOWN:
		s_move_down(snake);
		return 1;
	case SNAKE_MOVE_RIGHT:
		s_move_right(snake);
		return 1;
	case SNAKE_MOVE_LEFT:
		s_move_left(snake);
		return 1;
	default:
		return 0;
	}
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
}

short s_check_new_location(const snake *const snake, int x, int y)
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

void s_generate_food(snake *const snake)
{
	if (!snake) {
		return;
	}
	snake->food.x = rand() % (snake->max.x - 2) + 1;
	snake->food.y = rand() % (snake->max.y - 2) + 1;
}

short s_check_food(const snake *const snake)
{
	if (!snake) {
		return 0;
	}
	if (snake->head.x == snake->food.x && snake->head.y == snake->food.y) {
		return 1;
	}
	return 0;
}

void s_handle_food(snake *const snake, monitor *const monitor)
{
	if (!snake) {
		return;
	}

	dll_push_beginning(snake->body, &snake->head, sizeof(struct s_coordinates));
	if (s_check_food(snake)) {
		s_generate_food(snake);
		pthread_mutex_lock(&(monitor->mutex));
		monitor->signal_windows = SIGNAL_WINDOWS_REFRESH_FOOD;
		pthread_cond_signal(&(monitor->conditional));
		pthread_mutex_unlock(&(monitor->mutex));
	} else {
		s_coordinates *tail = (s_coordinates *)dll_pop_end(snake->body);
		if (!tail) {
			return;
		}
		free(tail);
	}
}
