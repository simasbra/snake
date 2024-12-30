/*
 * FILE: double_linked_list.h
 * TITLE: Double linked list library
 * AUTHOR: Simas Bradaitis <simasbra@proton.me>
 * VERSION: 0.1.0
 * DESCRIPTION: Contains all functions declarations for the double linked list management
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

#ifndef __DOUBLE_LINKED_LIST_H__
#define __DOUBLE_LINKED_LIST_H__

#include <stddef.h>

/*
 * Double linked list node
 */
typedef struct dll_node {
	void *data;
	struct dll_node *next;
	struct dll_node *previous;
} dll_node;

/*
 * Double linked list structure
 */
typedef struct double_linked_list {
	dll_node *head;
	dll_node *tail;
	size_t length;
} double_linked_list;

/*
 * Creates a new double linked list
 * \RETURNS: a pointer to newly created double linked list
 */
double_linked_list *dll_malloc(void);

/*
 * Frees the given double linked list
 */
void dll_free(double_linked_list **linked_list);

/*
 * Checks if the double linked list is empty
 * \RETURNS: 1 if empty, 0 if not
 */
int dll_is_empty(const double_linked_list *const linked_list);

/*
 * Gets current size of the double linked list
 * \RETURNS: current size
 */
size_t dll_get_length(const double_linked_list *const linked_list);

/*
 * Adds the given data to the beginning of the double linked list
 */
void dll_push_beginning(double_linked_list *const linked_list, void *data, size_t size);

/*
 * Adds the given data to the end of the double linked list
 */
void dll_push_end(double_linked_list *const linked_list, void *data, size_t size);

/*
 * Adds the given data at the given index of the double linked list
 */
void dll_push_index(double_linked_list *const linked_list, void *data, size_t size,
		    unsigned int index);

/*
 * Adds the given data to the end of the double linked list
 * \Alias for dll_push_end
 */
void dll_push(double_linked_list *const linked_list, void *data, size_t size);

/*
 * Removes the first element from the list, if the list is not empty
 * \RETURNS: pointer to removed data
 */
void *dll_pop_beginning(double_linked_list *const linked_list);

/*
 * Removes the last element from the list, if the list is not empty
 * \RETURNS: pointer to removed data
 */
void *dll_pop_end(double_linked_list *const linked_list);

/*
 * Removes an element at the given index from the list, if the list is not empty
 * \RETURNS: pointer to removed data
 */
void *dll_pop_index(double_linked_list *const linked_list, unsigned int index);

/*
 * Removes the last element from the list, if the list is not empty
 * \Alias for dll_pop_end
 * \RETURNS: pointer to removed data
 */
void *dll_pop(double_linked_list *const linked_list);

/*
 * Gets pointer to not modifiable element from the list at given index
 * \RETURNS: element at given index
 */
const void *dll_get_index(const double_linked_list *const linked_list, unsigned int index);

#endif
