/*
 * FILE: double_linked_list.c
 * TITLE: Double linked list library
 * AUTHOR: Simas Bradaitis <simasbra@proton.me>
 * VERSION: 0.1.0
 * DESCRIPTION: Contains all functions implementations for the double linked list management.
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

#include "double_linked_list.h"
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Allocates new node and copying given data
 */
dll_node *allocate_and_copy_data(void *data, size_t size);

/*
 * Checks whether double linked list is valid
 * \RETURNS: 1 if valid, 0 if not
 */
int check_double_linked_list(const double_linked_list *const linked_list);

/*
 * Checks arguments before adding to the double linked list
 * \RETURNS: 1 if ok, 0 if not
 */
int check_arguments_for_push(const double_linked_list *const linked_list, void *data, size_t size);

/*
 * Checks arguments before removing from the double linked list
 * \RETURNS: 1 if ok, 0 if not
 */
int check_arguments_for_pop(const double_linked_list *const linked_list);

/*
 * Checks where the given index is within double linked list bounds (0 < index < lenght + 1)
 * \RETURNS: 1 if it is, 0 if not
 */
int check_index_within_bounds(const double_linked_list *const linked_list, unsigned int index);

/*
 * Gets node at given index
 * \RETURNS: node
 */
dll_node *get_node_index(const double_linked_list *const linked_list, unsigned int index);

double_linked_list *dll_malloc(void)
{
	double_linked_list *list = (double_linked_list *)malloc(sizeof(double_linked_list));
	if (!list) {
		fprintf(stderr, "ERROR: malloc for linked list failed.\n");
		return NULL;
	}

	list->length = 0;
	list->head = NULL;
	list->tail = NULL;
	return list;
}

void dll_free(double_linked_list **linked_list)
{
	if (!linked_list) {
		fprintf(stderr, "ERROR: pointer to double linked list is NULL.\n");
		return;
	} else if (!check_double_linked_list(*linked_list)) {
		return;
	}

	while ((*linked_list)->length) {
		dll_pop(*linked_list);
	}
	free(*linked_list);
	*linked_list = NULL;
}

int dll_is_empty(const double_linked_list *const linked_list)
{
	if (!check_double_linked_list(linked_list)) {
		return 1;
	} else if (!linked_list->head || !linked_list->tail || linked_list->length == 0) {
		return 1;
	}
	return 0;
}

size_t dll_get_length(const double_linked_list *const linked_list)
{
	if (!check_double_linked_list(linked_list)) {
		return 0;
	}
	return linked_list->length;
}

void dll_push_beginning(double_linked_list *const linked_list, void *data, size_t size)
{
	if (!check_arguments_for_push(linked_list, data, size)) {
		return;
	}

	dll_node *new_node = allocate_and_copy_data(data, size);
	if (!new_node) {
		return;
	}

	new_node->previous = NULL;
	if (linked_list->head) {
		linked_list->head->previous = new_node;
		new_node->next = linked_list->head;
		linked_list->head = new_node;
	} else {
		new_node->next = NULL;
		linked_list->head = new_node;
		linked_list->tail = new_node;
	}
	linked_list->length++;
}

void dll_push_end(double_linked_list *const linked_list, void *data, size_t size)
{
	if (!check_arguments_for_push(linked_list, data, size)) {
		return;
	}

	dll_node *new_node = allocate_and_copy_data(data, size);
	if (!new_node) {
		return;
	}

	new_node->next = NULL;
	if (linked_list->tail) {
		new_node->previous = linked_list->tail;
		linked_list->tail->next = new_node;
	} else {
		new_node->previous = NULL;
		linked_list->head = new_node;
	}
	linked_list->tail = new_node;
	linked_list->length++;
}

void dll_push_index(double_linked_list *const linked_list, void *data, size_t size,
		    unsigned int index)
{
	if (!check_arguments_for_push(linked_list, data, size)) {
		return;
	} else if (!check_index_within_bounds(linked_list, index)) {
		return;
	}

	if (index == 0) {
		dll_push_beginning(linked_list, data, size);
	} else if (index == linked_list->length) {
		dll_push_end(linked_list, data, size);
	} else {
		dll_node *new_node = allocate_and_copy_data(data, size);
		if (!new_node) {
			return;
		}
		dll_node *current = get_node_index(linked_list, index);
		if (!current) {
			return;
		}
		new_node->previous = current->previous;
		new_node->next = current->previous->next;
		current->previous->next = new_node;
		current->previous = new_node;
		linked_list->length++;
	}
}

void dll_push(double_linked_list *const linked_list, void *data, size_t size)
{
	dll_push_end(linked_list, data, size);
}

void *dll_pop_end(double_linked_list *const linked_list)
{
	if (!check_arguments_for_pop(linked_list)) {
		return NULL;
	}

	dll_node *node = linked_list->tail;
	if (!linked_list->head->next) {
		linked_list->head = NULL;
		linked_list->tail = NULL;
	} else {
		linked_list->tail = node->previous;
	}

	void *data = node->data;
	free(node);
	linked_list->length--;
	return data;
}

void *dll_pop_beginning(double_linked_list *const linked_list)
{
	if (!check_arguments_for_pop(linked_list)) {
		return NULL;
	}

	dll_node *node = linked_list->head;
	if (!linked_list->head->next) {
		linked_list->head = NULL;
		linked_list->tail = NULL;
	} else {
		linked_list->head = linked_list->head->next;
	}

	void *data = node->data;
	free(node);
	linked_list->length--;
	return data;
}

void *dll_pop_index(double_linked_list *const linked_list, unsigned int index)
{
	if (!check_arguments_for_pop(linked_list)) {
		return NULL;
	} else if (!check_index_within_bounds(linked_list, index)) {
		return NULL;
	}

	if (index == 0) {
		return dll_pop_beginning(linked_list);
	} else if (index == linked_list->length) {
		return dll_pop_end(linked_list);
	} else {
		dll_node *current = get_node_index(linked_list, index);
		if (!current) {
			return NULL;
		}
		dll_node *node = current;
		current->next->previous = node->previous;
		current->previous->next = node->next;
		void *data = node->data;
		free(node);
		linked_list->length--;
		return data;
	}
}

void *dll_pop(double_linked_list *const linked_list)
{
	return dll_pop_end(linked_list);
}

const void *dll_get_index(const double_linked_list *const linked_list, unsigned int index)
{
	if (!check_double_linked_list(linked_list)) {
		return NULL;
	} else if (index < 0 || index > linked_list->length - 1) {
		fprintf(stderr, "ERROR: index is out of double linked list bounds");
		return NULL;
	}

	dll_node *current = get_node_index(linked_list, index);
	return current->data;
}

dll_node *allocate_and_copy_data(void *data, size_t size)
{
	dll_node *node = (dll_node *)malloc(sizeof(dll_node));
	if (!node) {
		fprintf(stderr, "ERROR: malloc for new node failed.\n");
		return NULL;
	}

	void *node_data = malloc(size);
	if (!node_data) {
		fprintf(stderr, "ERROR: malloc for new node datafailed.\n");
		free(node);
		return NULL;
	}

	memcpy(node_data, data, size);
	node->data = node_data;
	return node;
}

int check_arguments_for_push(const double_linked_list *const linked_list, void *data, size_t size)
{
	if (!check_double_linked_list(linked_list)) {
		return 0;
	} else if (!data) {
		fprintf(stderr, "ERROR: data is NULL.\n");
		return 0;
	} else if (size < 1) {
		fprintf(stderr, "ERROR: size is less than 1.\n");
		return 0;
	}
	return 1;
}

int check_arguments_for_pop(const double_linked_list *const linked_list)
{
	if (!check_double_linked_list(linked_list)) {
		return 0;
	} else if (dll_is_empty(linked_list)) {
		fprintf(stderr, "ERROR: double linked list is empty");
		return 0;
	}
	return 1;
}

int check_index_within_bounds(const double_linked_list *const linked_list, unsigned int index)
{
	if (index < 0 || index > linked_list->length) {
		fprintf(stderr, "ERROR: index is out of double linked list bounds");
		return 0;
	}
	return 1;
}

int check_double_linked_list(const double_linked_list *const linked_list)
{
	if (!linked_list) {
		fprintf(stderr, "ERROR: double linked list is NULL.\n");
		return 0;
	}
	return 1;
}

dll_node *get_node_index(const double_linked_list *const linked_list, unsigned int index)
{
	if (!check_index_within_bounds(linked_list, index)) {
		return NULL;
	}

	dll_node *current = NULL;
	if (index < linked_list->length / 2) {
		current = linked_list->head;
		for (size_t i = 0; i < linked_list->length && i != index; i++) {
			current = current->next;
		}
	} else {
		current = linked_list->tail;
		for (size_t i = linked_list->length - 1; i > 0 && i != index; i--) {
			current = current->previous;
		}
	}
	return current;
}
