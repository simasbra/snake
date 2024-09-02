#include "snake.h"
#include <stdlib.h>
#include <stdio.h>

Snake *s_new()
{
	struct Snake *snake = malloc(sizeof(struct Snake));
	if (!snake) {
		perror("Malloc failed\n");
		return NULL;
	}

	return snake;
}
