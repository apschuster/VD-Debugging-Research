#include "stack.h"

/* Register name faking - works in collusion with the linker.  */
register char * stack_ptr asm ("sp");
//an implementation of _sbrk to enable newlib's malloc
//THIS WILL MOST LIKELY NEED MORE TWEEKING
caddr_t __attribute__((weak))
_sbrk (int incr)
{
  extern char   end asm ("end");	/* Defined by the linker.  */
  static char * heap_end;
  char *        prev_heap_end;

  if (heap_end == NULL)
    heap_end = & end;
  
  prev_heap_end = heap_end;
  
  if (heap_end + incr > stack_ptr)
    {
      /* Some of the libstdc++-v3 tests rely upon detecting
	 out of memory errors, so do not abort here.  */
#if 0
      extern void abort (void);

      _write (1, "_sbrk: Heap and stack collision\n", 32);
      
      abort ();
#else
      //errno = ENOMEM;
      return (caddr_t) -1;
#endif
    }
  
  heap_end += incr;

  return (caddr_t) prev_heap_end;
}

//the constructor for the stack
string_stack* create()
{
	//dynamically assign memory of appropriate size to a new string_stack
	string_stack* result = (string_stack*) malloc(sizeof(string_stack));
	//assign enough memory to the stack initially for 10 strings
	result->stack = (char**) malloc(sizeof(char*) * 10);
	result->size = 10;//we have space for 10 entries
	result->stack_pointer = -1;//the stack is empty
	return result;
}

//the destructor for the stack
void destroy(string_stack* self)
{
	//go through the stack and deallocate all entries
	for(int c = 0; c < self->size; c++)
	{
		//note: stack[c] may be a nullptr if it hasn't been allocated
		free(self->stack[c]);
	}
	free(self->stack);//deallocate the stack itself
	free(self);//deallocate the whole string_stack structure
}

//replace the existing stack with a larger one
void resize(string_stack* self)
{
	//by default we will increase the size of the stack by 10
	self->size += 10;
	//stack should have the same contents, but be 10 entries larger
	self->stack = (char**) realloc(self->stack, sizeof(char*)*(self->size));
}

/*NOTE FOR THE FOLLOWING FUNCTIONS: 
* the item pointed to may, or may not have been initialized
* as such, these must account for both eventualities
* ALSO: all entries are dynamically allocated.
* As such, the pointers returned by these functions must be deallocated
*/

//set stack_pointer as one less, return previous top string
char* pop(string_stack* self)
{
	//make sure there's something to pop
	if(self->stack[self->stack_pointer] != NULL)
	{
		//store the top of the stack in result
		char* result = self->stack[self->stack_pointer];
		//deiterate stack_pointer
		self->stack_pointer--;
		return result;
	}
	return NULL;//if there's nothing on top of the stack, return NULL
}

//return the top string on the stack
char* top(string_stack* self)
{
	//make sure there's something to look at
	if(self->stack[self->stack_pointer] != NULL)
	{
		//store the top of the stack in result
		return self->stack[self->stack_pointer];
	}
	return NULL;//if there's nothing on top of the stack, return NULL
}

//iterate stack_pointer, insert entry at this location
void push(string_stack* self, char* entry)
{
	self->stack_pointer++;
	//determine the size of the entry by iterating until the end of entry
	int entry_size;
	for(entry_size = 1; entry[entry_size] != '\00'; entry_size++) {};
	//now we allocate enough space to the top of the stack for entry
	self->stack[self->stack_pointer] = (char*)malloc(entry_size);
	//now we copy entry into the top of the stack
	strcpy(self->stack[self->stack_pointer], entry);
}

//return 0 if stack is not full, return nonzero if it is full
char isFull(string_stack* self)
{
	//the stack is full if the stack pointer is greater than or equal to the size
	return self->stack_pointer >= (self->size);
}

//return 0 if stack is empty, return nonzero if it is empty
char isEmpty(string_stack* self)
{
	//stack is empty if stack pointer is less than 0
	return self->stack_pointer < 0;
}
