#ifndef STACK_H_
#define STACK_H_

#include<stdlib.h>
#include<stdbool.h>

struct Stack {
	unsigned stacksize;
	unsigned *items;
	unsigned *top;
};

void CreateStack(struct Stack *st, unsigned size){
	st->stacksize = size;
	st->items = (unsigned *)calloc(size, sizeof(unsigned));
	st->top = st->items;
}

bool isempty(struct Stack *st) {
	return (st->top == st->items);
}

bool push(struct Stack *st, unsigned item){
	if ((unsigned)(st->top - st->items) < st->stacksize) {
		*st->top++ = item;
		return true;
	}
	else return false;
}

bool pop(struct Stack *st){
	if (!isempty(st)) {
		--st->top;
		return true;
	}
	else return false;
}

bool top(struct Stack *st, unsigned *item){
	if (isempty(st))return false;
	else {
		*item = *(st->top - 1);
		return true;
	}
}

void DeleteStack(struct Stack *st){
	free(st->items);
	st->items = NULL;
	st->top = NULL;
}

#endif // !STACK_H_