/*
 * FILE: monitor.c
 * TITLE: Thread handling monitor
 * AUTHOR: Simas Bradaitis <simasbra@proton.me>
 * VERSION: 0.1.0
 * DESCRIPTION: Thread handling monitor implementation
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

#include "monitor.h"
#include <stdio.h>
#include <stdlib.h>

monitor *m_malloc(void)
{
	monitor *monitor = (struct monitor *)malloc(sizeof(struct monitor));
	if (!monitor) {
		fprintf(stderr, "ERROR: malloc failed for monitor allocation\n");
		return NULL;
	}
	if (pthread_mutex_init(&(monitor->mutex), NULL) != 0) {
		fprintf(stderr, "ERROR: mutex creation failed\n");
		free(monitor);
		return NULL;
	}
	monitor->signal = SIGNAL_EMPTY;

	return monitor;
}

void m_free(monitor **monitor)
{
	pthread_mutex_destroy(&((*monitor)->mutex));
	free(*monitor);
	*monitor = NULL;
}
