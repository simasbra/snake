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

#ifndef __MONITOR_H__
#define __MONITOR_H__

#include <pthread.h>

/*
 * Signal type when conditional is sent/received
 */
typedef enum m_signal_snake {
	SIGNAL_SNAKE_EMPTY,
	SIGNAL_SNAKE_GAME_EXIT,
	SIGNAL_SNAKE_MOVE,
	SIGNAL_SNAKE_COUNT
} m_signal_snake;

typedef enum m_signal_windows {
	SIGNAL_WINDOWS_EMPTY,
	SIGNAL_WINDOWS_GAME_EXIT,
	SIGNAL_WINDOWS_SNAKE_REFRESH,
	SIGNAL_WINDOWS_SNAKE_AND_FOOD_REFRESH,
	SIGNAL_WINDOWS_COUNT
} m_signal_windows;

/*
 * Snake move types
 */
typedef enum m_snake_move {
	SNAKE_MOVE_EMPTY,
	SNAKE_MOVE_UP,
	SNAKE_MOVE_DOWN,
	SNAKE_MOVE_RIGHT,
	SNAKE_MOVE_LEFT
} m_snake_move;

/*
 * Monitor shared between threads
 */
typedef struct monitor {
	pthread_mutex_t mutex;
	pthread_cond_t conditional;
	enum m_signal_snake signal_snake;
	enum m_signal_windows signal_windows;
	enum m_snake_move move_previous;
	enum m_snake_move move_next[2];
} monitor;

/*
 * Creates a new monitor object
 * \RETURNS: A newly created monitor
 */
monitor *m_malloc(void);

/*
 * Initializes monitor with default values
 */
void m_initialize(monitor *const monitor);

/*
 * Frees given monitor object
 */
void m_free(monitor **monitor);

#endif
