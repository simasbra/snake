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

typedef enum SignalType {
	EMPTY,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_RIGHT,
	MOVE_LEFT,
	GAME_RESUME,
	GAME_EXIT,
	SIGNAL_TYPE_COUNT
} Move;

typedef enum ThreadType { INPUT, GAME } ThreadType;

typedef struct Monitor {
	pthread_mutex_t mutex;
	pthread_cond_t input_received;
	enum SignalType signal_type;
} Monitor;

/*
 * Initializes input handling thread
 */
void *t_initalize_input(void **args);

/*
 * Initializes snake movement handling thread
 */
void *t_initalize_snake(void **args);

void t_initialize_threads(pthread_t *threads, Monitor *monitor);
