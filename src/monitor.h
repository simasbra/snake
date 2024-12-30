/*
 * FILE: monitor.h
 * TITLE: Thread handling monitor
 * AUTHOR: Simas Bradaitis <simasbra@proton.me>
 * VERSION: 0.1.0
 * DESCRIPTION: Thread handling monitor header file
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

#ifndef __MONITOR_H__
#define __MONITOR_H__

#include <pthread.h>

/*
 * Signal type when conditional is sent/received
 */
typedef enum m_signal {
	SIGNAL_EMPTY,
	SIGNAL_GAME_RESUME,
	SIGNAL_GAME_EXIT,
	SIGNAL_TYPE_COUNT
} m_signal;

/*
 * Snake move types
 */
typedef enum m_snake_move {
	SNAKE_MOVE_UP,
	SNAKE_MOVE_DOWN,
	SNAKE_MOVE_RIGHT,
	SNAKE_MOVE_LEFT
} m_snake_move;

/*
 * Thread types
 */
typedef enum m_thread { THREAD_INPUT, THREAD_GAME, THREAD_TYPE_COUNT } m_thread;

/*
 * Monitor shared between threads
 */
typedef struct monitor {
	pthread_mutex_t mutex;
	enum m_signal signal;
	enum m_snake_move move_previous;
	enum m_snake_move move_next;
} monitor;

/*
 * Creates a new monitor object
 * \RETURNS: A newly created monitor
 */
monitor *m_malloc(void);

/*
 * Frees given monitor object
 */
void m_free(monitor **monitor);

#endif
