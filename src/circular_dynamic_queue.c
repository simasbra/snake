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

#include "circular_dynamic_queue.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct circular_dynamic_queue *cdq_malloc(const size_t offset)
{
	if (offset < 1) {
		return NULL;
	}
	struct circular_dynamic_queue *queue = malloc(sizeof(struct circular_dynamic_queue));
	if (!queue) {
		perror("Circular dynamic queue memory allocation failed:\n");
		return NULL;
	}
	void *data = malloc(CDQ_INITIAL_SIZE * offset);
	if (!data) {
		perror("Circular dynamic queue data memory allocation failed:\n");
		free(queue);
		return NULL;
	}
	queue->data = data;
	queue->head = 0;
	queue->tail = 0;
	queue->size_current = 0;
	queue->size_max = CDQ_INITIAL_SIZE;
	queue->offset = offset;

	return queue;
}

struct circular_dynamic_queue *cdq_realloc(circular_dynamic_queue *const queue)
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

	for (size_t i = 0; i < queue->size_current; i++) {
		size_t index = (i + queue->head) % queue->size_max;
		memcpy((char *)new_data + (i * queue->offset),
		       (char *)queue->data + (index * queue->offset), queue->offset);
	}

	free(queue->data);
	queue->data = new_data;
	queue->size_max *= 2;
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

void cdq_push(struct circular_dynamic_queue *queue, const void *const new_data)
{
	if (!queue || !new_data) {
		return;
	}
	if (queue->size_current == queue->size_max) {
		circular_dynamic_queue *queue_realloc = cdq_realloc(queue);
		if (!queue_realloc->data) {
			return;
		}
		queue = queue_realloc;
	}

	size_t index;
	if (queue->size_current > 0 && (queue->tail != 0 || queue->tail + 1 < queue->size_max)) {
		index = queue->tail + 1;
	} else {
		index = 0;
	}
	memcpy((char *)queue->data + index * queue->offset, new_data, queue->offset);
	queue->size_current++;
	queue->tail = index;
}

void cdq_pop(struct circular_dynamic_queue *const queue)
{
	if (!queue) {
		return;
	}
	if (queue->size_current == 0) {
		return;
	}
	memset((char *)queue->data + queue->head * queue->offset, 0, queue->offset);
	if (queue->size_current + 1 == queue->size_max) {
		queue->head = 0;
	} else {
		queue->head++;
	}
	queue->size_current--;
}

const void *cdq_head(const struct circular_dynamic_queue *const queue)
{
	if (!queue) {
		return NULL;
	}
	if (queue->size_current == 0) {
		return NULL;
	}

	return (char *)queue->data + (queue->head * queue->offset);
}
