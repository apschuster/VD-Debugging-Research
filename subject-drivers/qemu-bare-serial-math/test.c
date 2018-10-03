//#include <stdint.h>
//#include <stdlib.h>
#include "stack.h"

//The command codes
const char additionCode = 1;
const char subtractionCode = 2;
const char multiplicationCode = 3;
const char divisionCode = 4;
const char modulusCode = 5;

enum {
    RXFE = 0x10,//recieve flag
    TXFF = 0x20,//transmit flag
};

typedef volatile struct {
    uint32_t DR;//data register
    uint32_t RSR_ECR;
    uint8_t reserved1[0x10];
    const uint32_t FR;
    uint8_t reserved2[0x4];
    uint32_t LPR;
    uint32_t IBRD;
    uint32_t FBRD;
    uint32_t LCR_H;
    uint32_t CR;    
    uint32_t IFLS;
    uint32_t IMSC;
    const uint32_t RIS;
    const uint32_t MIS;
    uint32_t ICR;
    uint32_t DMACR;    
} pl011_T;


pl011_T * const UART0 = (pl011_T *)0x101f1000;
//pl011_T * const UART1 = (pl011_T *)0x101f2000;
//pl011_T * const UART2 = (pl011_T *)0x101f3000;

static inline char upperchar(char c) {
    if((c >= 'a') && (c <= 'z')) {
        return c - 'a' + 'A';
    } else {
        return c;
    }
}

/*
Since our toolchain doesn't support the c standard library version of strcmp
I am implimenting a simple recurrsive version here
*/
int strcmp(const char* a, const char* b)
{
	if (! (*a | *b)) 
		return 0;
	return (*a != *b) ? *a-*b : strcmp(++a, ++b);
}

volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;

void print_uart0(const char *s) {
	while(*s != '\0') { /* Loop until end of string */
		*UART0DR = *s; /* Transmit char */
		s++; /* Next char */
	}
}

//the base command control function
int run_stack(string_stack stack)
{
	//the top of the stack should always be a command when this is called
	if( ! char commandCode = isCommand(stack->pop(stack)) )
		//if not, return 0
		return 0;

	switch (commandCode)
	{
		//we now check the command code against the operation codes
		case subtractionCode:
			//have the subtract function handle the two operands
			return subtract(stack);
			break;
		case additionCode:
			//have the addition function handle the two operands
			return add(stack);
			break;
		case multiplicationCode:
			//have the multiply function handle the two operands
			return multiply(stack);
			break;
		case divisionCode:
			//have the division function handle the two operands
			return divide(stack);
			break;
		case modulusCode:
			//have the modulus function handle the two operands
			return modulus(stack);
			break;
		default:
			return 0;//if there is no match, return 0
	}
}

//begin operations
//note: the command itself should have been popped off,
//we need only deal with the two operands
int subtract(string_stack stack)
{
	int operand[2];
	if( ! isNum(stack->top(stack)) )//if the top of stack is not numerical
		return 0;
	operand[0] = stack->pop(stack);//put the first operand where it belongs
	
	if( ! isNum(stack->top(stack)) )//if the second operand is nonnumerical
		operand[1] = run_stack(stack);//assume top is a command
		//note: this is how the operations are chained together
	
	return operand[0] - operand[1];//subtract the two operands
}

int add(string_stack stack)
{
	int operand[2];
	if( ! isNum(stack->top(stack)) )//if the top of stack is not numerical
		return 0;
	operand[0] = stack->pop(stack);//put the first operand where it belongs
	
	if( ! isNum(stack->top(stack)) )//if the second operand is nonnumerical
		operand[1] = run_stack(stack);//assume top is a command
		//note: this is how the operations are chained together
	
	return operand[0] + operand[1];//add the two operands
}

int multiply(string_stack stack)
{
	int operand[2];
	if( ! isNum(stack->top(stack)) )//if the top of stack is not numerical
		return 0;
	operand[0] = stack->pop(stack);//put the first operand where it belongs
	
	if( ! isNum(stack->top(stack)) )//if the second operand is nonnumerical
		operand[1] = run_stack(stack);//assume top is a command
		//note: this is how the operations are chained together
	
	return operand[0] * operand[1];//multiply the two operands
}

int divide(string_stack stack)//this may require additional libraries
{
	int operand[2];
	if( ! isNum(stack->top(stack)) )//if the top of stack is not numerical
		return 0;
	operand[0] = stack->pop(stack);//put the first operand where it belongs
	
	if( ! isNum(stack->top(stack)) )//if the second operand is nonnumerical
		operand[1] = run_stack(stack);//assume top is a command
		//note: this is how the operations are chained together
	
	return operand[0] / operand[1];//divide the two operands
}

int modulus(string_stack stack)
{
	int operand[2];
	if( ! isNum(stack->top(stack)) )//if the top of stack is not numerical
		return 0;
	operand[0] = stack->pop(stack);//put the first operand where it belongs
	
	if( ! isNum(stack->top(stack)) )//if the second operand is nonnumerical
		operand[1] = run_stack(stack);//assume top is a command
		//note: this is how the operations are chained together
	
	return operand[0] % operand[1];//mod the two operands
}
//end operations

//This function checks if the given command is a valid command entry
//If the entry is a command, it will return the appropriate command value
//Otherwise, it will return 0
char isCommand(char *command)
{
	/*
	acceptable strings for addition:
		"+"
		"ADD"
	*/
	if( (strcmp(command, "+") == 0) || (strcmp(command, "ADD") == 0) )
		return additionCode;//uses constant for addition value
	/*
	acceptable strings for subtraction:
		"-"
		"SUB"
		"SUBTRACT"
	*/
	if( (strcmp(command, "-") == 0) || (strcmp(command, "SUB") == 0) || (strcmp(command, "SUBTRACT") == 0) )
		return subtractionCode;
	/*
	acceptable strings for multiplication:
		"*"
		"MUL"
		"MULTIPLY"
	*/
	if( (strcmp(command, "*") == 0) || (strcmp(command, "MUL") == 0) || (strcmp(command, "MULTIPLY") == 0) )
		return multiplicationCode;
	/*
	acceptable strings for division:
		"/"
		"DIV"
		"DIVIDE"
	*/
	if( (strcmp(command, "/") == 0) || (strcmp(command, "DIV") == 0) || (strcmp(command, "DIVIDE") == 0) )
		return divisionCode;
	/*
	acceptable strings for modulus:
		"%"
		"MOD"
		"MODULUS"
	*/
	if( (strcmp(command, "%") == 0) || (strcmp(command, "MOD") == 0) || (strcmp(command, "MODULUS") == 0) )
		return modulusCode;
	return 0;
}
//this function checks if a given command is only numerical
char isNum(char *command)
{
	char result = 1;//result starts true
	//result is &'d with whether each char in command is numerical
	for(int c = 0; command[c] != '\00'; c ++)
		result &= (command[c] < 58 && command[c] > 47);
	return result;
}
//This function checks if a given string is a valid entry
//return zero if invalid, nonzero otherwise
char isValid(char *command)
{
	return isCommand(command) && isNum(command);
}
    
//This loop will handle the buffer I/O
static int uart_echo_buffer(pl011_T *uart, char* buffer, int position, string_stack *stack) 
{
    if ((uart->FR & RXFE) == 0) 
	{
        	while(uart->FR & TXFF) { };
		buffer[position] = upperchar(uart->DR);

		//it may be simpler to pick a different deletion character
		if (buffer[position] == 8)
		{
			//make sure we don't delete beyond current line
			if(position > 0)
			{
				//replace previous printed character with blank
				print_uart0("\b\00");
				//blank current and previous character
				buffer[position] = '\00';
				position--;
				buffer[position] = '\00';
			}
			return position;
		}

		//if we recieve the submission character (Enter) or we reach the end of buffer
		//note that we need a zero byte at the end of the buffer so we reset once we fill the last bit in the buffer
		else if( (buffer[position] == '\13') || (position > 98) )
		{
			if(strcmp(buffer, "QUIT\13") == 0)
			{
				print_uart0("Exiting Program\n");
				return(-1);
			}
			//RUN COMMANDS ON STACK
			else if (strcmp(buffer, "RUN\13") == 0)
			{
				//store results of instructions on stack here
				int result = run_stack(stack);
				int result_len = 2;//store decimal length here
				for(int pow = 10; result >= pow; pow *= 10)
				{
					/*
					add one to result length for each
					increasing power of ten that is
					less than the result
					*/
					result_len++;
					//this gives result_len the necessary length for a string to contain the conversion of result
				}
				
				char result_str[result_len];
				for(int i = 0; i < result_len; i++)
					result_str[i] = '\00';//zero the string
				
				//convert int to string
				itoa(result, result_str, 10);
				//print result
				print_uart0("Result: ");
				print_uart0((const char*) result_str);
				print_uart0("\n");
			}

			//place buffered command in stack
			if(isValid(buffer))
			{
				//Strip unparsable characters and whitespace
				for(int i = 0; buffer [i] != '\00'; i++)
					if(buffer[i] < 33)
						buffer[i] = '\00';
				stack->push(stack, buffer);
			}

			//zero out buffer
			for(int c = 0; c < 100; c++)
			{
				buffer[c] = '\0';
			}

			//PRINT THE LAST LINE SUBMITTED
			print_uart0("Line Recorded\n");
			char* lastSubmission;
			strcpy(lastSubmission, stack->top(stack));
			print_uart0((const char*)lastSubmission);
			print_uart0("\n");
			return 0;
		}

		//TO DO: REPLACE CURRENT LINE RATHER THAN PRINT NEW ONE
		position++;//iterate the position placement
		//we will try to delete the previously printed line by sending the corrisponding VT100 escape code 
		print_uart0("27[2K");
		const char *printBuffer = (const char *)buffer;
		print_uart0(printBuffer);
	}
	return position;
}

void c_entry() 
{
	//TO DO: make proper instructions
	print_uart0("This program will store a buffer of your input.\n");
	print_uart0("All inputs will be capitalized.\n");
	print_uart0("The buffered stirng will be added to a stack upon pressing enter.\n");
	print_uart0("The stack can be run by entering \"RUN\".\n");
	print_uart0("If you wish to quit the program, enter \"QUIT\" and submit.\n");

	char buffer[100];
	int position;
	string_stack* instruction_stack;
	instruction_stack = instruction_stack->create();
	//zero out the buffer
	for(int c = 0; c < 100; c++)
	{
		buffer[c] = '\0';
	}
	for(;;) 
	{
		position = uart_echo_buffer(UART0, buffer, position, instruction_stack);
		if(position < 0)
		{
			instruction_stack->destroy(instruction_stack);
			return;
		}
	}
	instruction_stack->destroy(instruction_stack);//make sure to deallocate
}

