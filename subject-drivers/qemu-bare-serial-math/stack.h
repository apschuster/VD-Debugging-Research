#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

typedef struct
{	
	struct string_stack* (*create) ();
	void (*destroy) (struct string_stack* self);
	void (*resize) (struct string_stack* self);
	char* (*pop) (struct string_stack* self);
	char* (*top) (struct string_stack* self);
	char* (*push) (struct string_stack* self, char* entry);
	char (*isFull) (struct string_stack* self);
	char (*isEmpty) (struct string_stack* self);
	int size;//for storing the current maximum number of strings in stack
	int stack_pointer;//for storing the index of the current top of stack
	char** stack;//the stack itself is an array of pointers, dynamically allocated
} string_stack;

//the constructor for the stack
string_stack* create();

//the destructor for the stack
void destroy(string_stack* self);

//replace the existing stack with a larger one
void resize(string_stack* self);

//set stack_pointer as one less, return previous top string
char* pop(string_stack* self);

//return the top string on the stack
char* top(string_stack* self);

//iterate stack_pointer, insert entry at this location
void push(string_stack* self, char* entry);

//return 0 if stack is not full, return nonzero if it is full
char isFull(string_stack* self);

//return 0 if stack is empty, return nonzero if it is empty
char isEmpty(string_stack* self);

#endif
