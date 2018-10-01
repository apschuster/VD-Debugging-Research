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

//TO DO: CREATE RUN FUNCTION
int run_stack(string_stack stack)
{

}
//TO DO: CREATE OPERAND FUNCTIONS
//begin operands
int subtract(string_stack stack)
{

}

int add(string_stack stack)
{

}

int multiply(string_stack stack)
{

}

int divide(string_stack stack)//this may require additional libraries
{

}

int modulus(string_stack stack)
{

}
//end operands

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
		"MODULUD"
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
			//TO DO: RUN COMMANDS ON STACK
			else if (strcmp(buffer, "RUN\13") == 0)
			{
				
			}

			//place buffered command in stack
			if(isValid(buffer))
			{
				//Strip enter character
				for(int i = 0; buffer [i] != '\00'; i++)
					if(buffer[i] == '\13')
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

		/*TO DO: REMOVE
		if((buffer[position] == ':'))
		{
			buffer[position] = 0;
			int sum = 0;
			for(int c = 0; c < position; c++)
			{
				sum += buffer[c];
			}
			//make sure the buffer has enough space for the digits
			char stringSum[(sum/10)+2];
			for(int c = 0; c < 100; c++)//zero out the buffer
			{
				stringSum[c] = '\0';
			}
			itoa(sum, stringSum, 10);
			const char *printSum = (const char *) stringSum;
			print_uart0(printSum);
			print_uart0("\n");
			return position;
		}*/

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
	print_uart0("Entering a \";\" character will submit the buffer to the stack.\n");
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
			return;
		}
	}
}

