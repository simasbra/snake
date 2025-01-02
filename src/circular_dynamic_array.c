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

#include "circular_dynamic_array.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct circular_dynamic_queue *const cdq_malloc(const size_t offset)
{
	if (offset < 1) {
		return NULL;
	}
	struct circular_dynamic_queue *queue = malloc(sizeof(struct circular_dynamic_queue));
	if (!queue) {
		perror("Circular dynamic queue memory allocation failed:\n");
		return NULL;
	}
	void *data = malloc(sizeof(CDQ_INITIAL_SIZE * offset));
	if (!data) {
		perror("Circular dynamic queue data memory allocation failed:\n");
		free(queue);
		return NULL;
	}
	queue->head = 0;
	queue->tail = 0;
	queue->size_current = 0;
	queue->size_max = CDQ_INITIAL_SIZE;
	queue->offset = offset;

	return queue;
}

struct circular_dynamic_queue *const cdq_realloc(circular_dynamic_queue *const queue)
{
	if (!queue || !queue->data) {
		return queue;
	}
	size_t new_size = queue->size_max * queue->offset * 2;
	void *new_data = malloc(new_size);
	if (!new_data) {
		perror("Circular dynamic queue data memory reallocation failed:\n");
		return queue;
	}

	for (int i = 0; i < queue->size_current; i++) {
		size_t index = (i + queue->head) % queue->size_max;
		memcpy(new_data + (i * queue->offset), queue->data + (index * queue->offset),
		       queue->offset);
	}

	free(queue->data);
	queue->data = new_data;
	queue->size_max = new_size;
	queue->head = 0;
	queue->tail = queue->size_current - 1;
	return queue;
}

void cdq_free(struct circular_dynamic_queue **queue)
{
	if (!queue || !*queue) {
		return;
	}
	free((*queue)->data);
	(*queue)->data = NULL;
	free(*queue);
	*queue = NULL;
}
