#include <stdint.h>
#include <stdlib.h>

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
    
//modify this so that it stores a buffer and clears it on a ';'
static int uart_echo_buffer(pl011_T *uart, char* buffer, int position) {
    if ((uart->FR & RXFE) == 0) 
	{
        while(uart->FR & TXFF) { };
		buffer[position] = upperchar(uart->DR);

		//if we recieve the clearing character or we reach the end of buffer
		//note that we need a zero byte at the end of the buffer so we
		//reset once we fill the last bit in the buffer
		if( (buffer[position] == ';') || (position > 98) )
		{
			if(strcmp(buffer, "QUIT;") == 0)
			{
				print_uart0("Exiting Program\n");
				return(-1);
			}
			//zero out buffer
			for(int c = 0; c < 100; c++)
			{
				buffer[c] = '\0';
			}
			print_uart0("Clearing Buffer\n");
			//return position = 0
			return 0;
		}
		//if we recieve the summing character we return the sum of the characters in the buffer
		if((buffer[position] == ':'))
		{
			buffer[position] = 0;
			int sum = 0;
			for(int c = 0; c < position; c++)
			{
				sum += buffer[c];
			}
			//DO A PROPER TRANSLATION OF SUM INTO STRING
			char stringSum[(sum/10)+2];
			itoa(sum, stringSum, 10);
			const char *printSum = (const char *) stringSum;
			print_uart0(printSum);
			print_uart0("\n");
			return position;
		}
		position++;//iterate the position placement
		const char *printBuffer = (const char *)buffer;
		print_uart0(printBuffer);
		print_uart0("\n");
	}
	return position;
}

void c_entry() 
{
	print_uart0("This program will store a buffer of your input and echo it back to you after every new input.\n");
	print_uart0("All inputs will be capitalized.\n");
	print_uart0("Entering a \";\" character will clear the buffer.\n");
	print_uart0("If you wish to quit the program, clear the buffer and enter \"QUIT\" and clear it again.\n");

	char buffer[100];
	int position;
	//zero out the buffer
	for(int c = 0; c < 100; c++)
	{
		buffer[c] = '\0';
	}
	for(;;) {
		position = uart_echo_buffer(UART0, buffer, position);
		if(position < 0)
		{
			return;
		}
	}
}

