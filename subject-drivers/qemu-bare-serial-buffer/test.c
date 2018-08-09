#include <stdint.h>

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

volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;

void print_uart0(const char *s) {
	while(*s != '\0') { /* Loop until end of string */
		*UART0DR = *s; /* Transmit char */
		s++; /* Next char */
	}
}
    
//modify this so that it stores a buffer and clears it on a ';'
static void uart_echo_buffer(pl011_T *uart, char* buffer, int* position) {
    if ((uart->FR & RXFE) == 0) {
        while(uart->FR & TXFF);
        buffer[*position] = upperchar(uart->DR);
		*position++;//iterate the position placement
    }
	const char *printBuffer = (const char *)buffer;
	print_uart0(printBuffer);
	/*
	int iterations = 0;
	char* s = buffer;//set pointer to start of buffer
	while(*s != '\0' && iterations < 100) { //Loop until end of string
		uart->DR = *s; //Transmit char
		s++; //Next char
		iterations++;//make sure to count iterations so as not to overflow buffer
	}
	*/
}

void c_entry() {
	//MAKE SURE TO CHECK MAX SIZE
	char buffer[100];
	int position;
    for(;;) {
        uart_echo_buffer(UART0, buffer, &position);
        //uart_echo(UART1);
        //uart_echo(UART2);
    }
}

