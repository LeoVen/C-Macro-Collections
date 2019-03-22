#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../src/macro_containers.h"

CONTAINER_GENERATE(LIST, PUBLIC, l, list, static, , int)
CONTAINER_GENERATE(STACK, PUBLIC, s, stack, static, , int)
CONTAINER_GENERATE(QUEUE, PUBLIC, q, queue, static, , int)
CONTAINER_GENERATE(DEQUE, PUBLIC, d, deque, static, , int)

int main(int argc, char const *argv[])
{
	list *l = l_new(100);
	stack *s = s_new(100);
	queue *q = q_new(100);
	deque *d = d_new(100);

	for (int i = 0; i < 20; i++)
	{
		l_push_back(l, i);
		s_push(s, i);
		q_enqueue(q, i);
		d_push_back(d, i);
	}

	s_push(s, 21);
	q_enqueue(q, 21);
	q_enqueue(q, 22);
	d_push_back(d, 21);
	d_push_back(d, 22);
	d_push_back(d, 23);

	FOR_EACH(l, list, , int, l, {
		if (index == 0)
			printf("[ %d, ", var);
		else if (index == l_count(l) - 1)
			printf("%d ]\n", var);
		else
			printf("%d, ", var);
	})

	FOR_EACH(s, stack, , int, s, {
		if (index == 0)
			printf("[ %d, ", var);
		else if (index == s_count(s) - 1)
			printf("%d ]\n", var);
		else
			printf("%d, ", var);
	})

	FOR_EACH(q, queue, , int, q, {
		if (index == 0)
			printf("[ %d, ", var);
		else if (index == q_count(q) - 1)
			printf("%d ]\n", var);
		else
			printf("%d, ", var);
	})

	FOR_EACH(d, deque, , int, d, {
		if (index == 0)
			printf("[ %d, ", var);
		else if (index == d_count(d) - 1)
			printf("%d ]\n", var);
		else
			printf("%d, ", var);
	})

	l_free(l);
	s_free(s);
	q_free(q);
	d_free(d);

	return 0;
}
