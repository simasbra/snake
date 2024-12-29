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

#include <pthread.h>
#include <stdlib.h>

typedef enum SignalType {
	SIGNAL_EMPTY,
	SIGNAL_MOVE_UP,
	SIGNAL_MOVE_DOWN,
	SIGNAL_MOVE_RIGHT,
	SIGNAL_MOVE_LEFT,
	SIGNAL_GAME_RESUME,
	SIGNAL_GAME_EXIT,
	SIGNAL_TYPE_COUNT
} Move;

typedef enum ThreadType { THREAD_INPUT, THREAD_GAME, THREAD_TYPE_COUNT } ThreadType;

typedef struct Monitor {
	pthread_mutex_t mutex;
	pthread_cond_t input_received;
	enum SignalType signal_type;
} Monitor;

/*
 * Creates a new monitor object
 * \RETURNS: A newly created monitor
 */
Monitor *m_malloc(void);

/*
 * Frees given monitor object
 */
void m_free(Monitor **monitor);
