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
#include <stdlib.h>
#include <stdio.h>

snake *s_malloc(void)
{
	struct snake *snake = malloc(sizeof(struct snake));
	if (!snake) {
		perror("ERROR: Snake malloc failed\n");
		return NULL;
	}
	snake->body = cdq_malloc(sizeof(s_coordinates));
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
	s_coordinates tail = { -1, -1 };

	snake->head = head;
	snake->max = max;
	snake->tail = tail;
	snake->score = 0;
	s_generate_food(snake);
	cdq_push(snake->body, &head);
}

void s_free(snake **snake)
{
	if (!snake) {
		fprintf(stderr, "ERROR: snake is NULL.\n");
		return;
	}

	cdq_free(&((*snake)->body));
	(*snake)->body = NULL;
	free(*snake);
	*snake = NULL;
}

void s_move(snake *const snake, monitor *const monitor)
{
	if (!snake || !monitor) {
		return;
	}

	struct timespec sleep_time;
	sleep_time.tv_sec = 0;
	sleep_time.tv_nsec = 1e8;

	while (1) {
		pthread_mutex_lock(&(monitor->mutex));
		if (s_handle_signal(snake, monitor)) {
			pthread_mutex_unlock(&(monitor->mutex));
			return;
		}
		pthread_mutex_unlock(&(monitor->mutex));

		if (!monitor->snake_alive) {
			s_remove_snake_tail(snake);
			s_signal_windows(monitor, SIGNAL_WINDOWS_SNAKE_DIED);
			return;
		}

		cdq_push(snake->body, &snake->head);
		if (s_handle_food(snake)) {
			s_signal_windows(monitor, SIGNAL_WINDOWS_SNAKE_AND_FOOD_REFRESH);
		} else {
			s_remove_snake_tail(snake);
			s_signal_windows(monitor, SIGNAL_WINDOWS_SNAKE_REFRESH);
		}
		nanosleep(&sleep_time, NULL);
	}
}

short s_handle_signal(snake *const snake, monitor *const monitor)
{
	if (!snake || !monitor) {
		return 0;
	}
	switch (monitor->signal_snake) {
	case SIGNAL_SNAKE_GAME_EXIT:
		monitor->signal_snake = SIGNAL_SNAKE_EMPTY;
		return 1;
	case SIGNAL_SNAKE_MOVE:
		s_handle_move(snake, monitor);
		return 0;
	default:
		return 0;
	}
}

void s_handle_move(snake *const snake, monitor *const monitor)
{
	if (!snake || !monitor) {
		return;
	}

	enum m_snake_move move = SNAKE_MOVE_EMPTY;
	if (monitor->move_next[0] != SNAKE_MOVE_EMPTY) {
		move = monitor->move_next[0];
	} else {
		move = monitor->move_previous;
	}

	short valid_move = 0;
	switch (move) {
	case SNAKE_MOVE_UP: {
		s_coordinates offset = { 0, -1 };
		valid_move = s_move_direction(snake, offset);
		break;
	}
	case SNAKE_MOVE_DOWN: {
		s_coordinates offset = { 0, 1 };
		valid_move = s_move_direction(snake, offset);
		break;
	}
	case SNAKE_MOVE_RIGHT: {
		s_coordinates offset = { 1, 0 };
		valid_move = s_move_direction(snake, offset);
		break;
	}
	case SNAKE_MOVE_LEFT: {
		s_coordinates offset = { -1, 0 };
		valid_move = s_move_direction(snake, offset);
		break;
	}
	default:
		return;
	}

	if (!valid_move) {
		monitor->snake_alive = 0;
		return;
	}
	monitor->move_next[0] = monitor->move_next[1];
	monitor->move_next[1] = SNAKE_MOVE_EMPTY;
	monitor->move_previous = move;
}

short s_move_direction(snake *const snake, const s_coordinates offset)
{
	if (!snake) {
		return 0;
	}
	if (offset.x < -1 || offset.x > 1 || offset.y < -1 || offset.y > 1) {
		return 0;
	}
	short valid_move = 1;
	if (!s_check_new_location(snake, snake->head.x + offset.x, snake->head.y + offset.y)) {
		valid_move = 0;
	}
	snake->head.x += offset.x;
	snake->head.y += offset.y;
	return valid_move;
}

short s_check_new_location(const snake *const snake, int x, int y)
{
	if (!snake) {
		return 0;
	}
	if (x < 1 || y < 1 || x > snake->max.x - 2 || y > snake->max.y - 2) {
		return 0;
	}
	for (unsigned int i = 0; i < snake->body->size_current; i++) {
		s_coordinates *body_part = (struct s_coordinates *)cdq_index(snake->body, i);
		if (x == body_part->x && y == body_part->y) {
			return 0;
		}
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

short s_handle_food(snake *const snake)
{
	if (!snake) {
		return 0;
	}
	if (s_check_food(snake)) {
		s_generate_food(snake);
		snake->score++;
		return 1;
	}
	return 0;
}

void s_signal_windows(monitor *const monitor, enum m_signal_windows signal)
{
	if (!monitor || signal == SIGNAL_WINDOWS_EMPTY) {
		return;
	}
	pthread_mutex_lock(&(monitor->mutex));
	monitor->signal_windows = signal;
	pthread_cond_signal(&(monitor->conditional));
	pthread_mutex_unlock(&(monitor->mutex));
}

void s_remove_snake_tail(snake *const snake)
{
	s_coordinates *tail = (s_coordinates *)cdq_head(snake->body);
	if (!tail) {
		return;
	}
	snake->tail = *tail;
	cdq_pop(snake->body);
}
