#include "circular_dynamic_queue.h"
#include <stddef.h>
#include <stdio.h>

void print_all(const circular_dynamic_queue *const queue);

int main(void)
{
	circular_dynamic_queue *queue = cdq_malloc(sizeof(int));
	int data[3] = { 10, 20, 30 };
	cdq_push(queue, &(data[0]));
	cdq_push(queue, &(data[1]));
	cdq_push(queue, &(data[2]));
	print_all(queue);

	cdq_pop(queue);
	print_all(queue);

	cdq_push(queue, &(data[0]));
	print_all(queue);

	cdq_pop(queue);
	print_all(queue);

	cdq_push(queue, &(data[1]));
	print_all(queue);

	cdq_free(&queue);
	return 0;
}

void print_all(const circular_dynamic_queue *const queue)
{
	if (!queue) {
		return;
	}
	for (size_t i = 0; i < queue->size_current; i++) {
		printf("%d, ", *((int *)cdq_index(queue, i)));
	}
	printf("\n");
}
