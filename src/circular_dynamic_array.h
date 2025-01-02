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

#ifndef __CIRCULAR_DYNAMIC_ARRAY_H__
#define __CIRCULAR_DYNAMIC_ARRAY_H__

#define CDQ_INITIAL_SIZE 2

#include <stddef.h>

/*
 * Circular dynamic queue.
 * Head is the index of the "first" element,
 * tail is the index of the "last" element in the queue.
 */
typedef struct circular_dynamic_queue {
	size_t head;
	size_t tail;
	size_t size_current;
	size_t size_max;
	size_t offset;
	void *data;
} circular_dynamic_queue;

/*
 * Allocates and initializes new circular dynamic queue
 * \RETURNS: pointer to the newly created queue
 */
struct circular_dynamic_queue *const cdq_malloc(const size_t offset);

/*
 * Reallocates more memory for data and copies its data to the new memory.
 * Also rearrages elements if needed
 * \RETURNS: pointer to the queue
 */
struct circular_dynamic_queue *const cdq_realloc(struct circular_dynamic_queue *const queue);

/*
 * Frees the given circular dynamic queue
 */
void cdq_free(struct circular_dynamic_queue **queue);

#endif
