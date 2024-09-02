#ifndef SNAKE_H
#define SNAKE_H

/*
 * Struct for storing snake information
 */
typedef struct Snake {
	int x_head, y_head;
	unsigned int lenght;
} Snake;

Snake *s_new();

#endif
