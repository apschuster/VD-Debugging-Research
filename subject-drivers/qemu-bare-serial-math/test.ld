ENTRY(_Reset)
SECTIONS
{
	. = 0x10000; 
	.text : {
			startup.o
			*(.text)
		}
	.data : { *(.data) }
	.bss : { *(.bss) }
	.heap : /* This part will provide the heap for newlib's malloc*/
	{
		__end__ = .;
		PROVIDE(end = .);
		PROVIDE(_heap_start = end);
		. = . + 0x800;/* 2kB of heap memory*/
		PROVIDE(_heap_end = .);
	}
	. = . + 0x800; /* 2kB of stack memory */
	stack_top = .;
}

