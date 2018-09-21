#include "stack.h"

/* Register name faking - works in collusion with the linker.  */
register char * stack_ptr asm ("sp");
//an implementation of _sbrk to enable newlib's malloc
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
	result->size = 10;
	result->stack_pointer = 0;//point to bottom of the stack
}

//the destructor for the stack
void destroy(string_stack* self)
{

}

//replace the existing stack with a larger one
void resize(string_stack* self)
{

}

//NOTE FOR THE FOLLOWING FUNCTIONS: 
//the item pointed to may, or may not have been initialized
//as such, these must account for both eventualities

//set stack_pointer as one less, return previous top string
char* pop(string_stack* self)
{

}

//return the top string on the stack
char* top(string_stack* self)
{

}

//iterate stack_pointer, insert entry at this location
void push(string_stack* self, char* entry)
{

}

//return 0 if stack is not full, return nonzero if it is full
char isFull(string_stack* self)
{

}

//return 0 if stack is empty, return nonzero if it is empty
char isEmpty(string_stack* self)
{

}
