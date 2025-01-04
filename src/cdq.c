#include "circular_dynamic_queue.h"
#include <stdio.h>

int main(void)
{
	circular_dynamic_queue *queue = cdq_malloc(sizeof(int));
	int data[3] = { 1, 2, 3 };
	cdq_push(queue, &(data[0]));

	cdq_free(&queue);
	return 0;
}
