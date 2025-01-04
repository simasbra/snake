#include "circular_dynamic_queue.h"
#include <stdio.h>

int main(void)
{
	circular_dynamic_queue *queue = cdq_malloc(sizeof(int));
	int data[3] = { 10, 20, 30 };
	cdq_push(queue, &(data[0]));
	cdq_push(queue, &(data[1]));
	cdq_push(queue, &(data[2]));
	int *head = cdq_head(queue);
	printf("%d", *head);

	cdq_free(&queue);
	return 0;
}
