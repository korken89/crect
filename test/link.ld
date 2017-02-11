/* Link file for STM32F411xe */

MEMORY
{
	ROM (rx)  : ORIGIN = 0x08000000, LENGTH = 512k
	RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

ENTRY(Reset_Handler)

SECTIONS
{
	.text : {
		*(.isr_vectors)
		*(.text*)
		. = ALIGN(4);
		*(.rodata*)
		. = ALIGN(4);
	} > ROM

	.preinit_array : {
		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP (*(.preinit_array))
		__preinit_array_end = .;
	} > ROM

	.init_array : {
		. = ALIGN(4);
		__init_array_start = .;
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		__init_array_end = .;
	} > ROM

	.fini_array : {
		. = ALIGN(4);
		__fini_array_start = .;
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
		__fini_array_end = .;
	} > ROM

	. = ALIGN(4);
	__text_end = .;

	.data : {
		__data_start = .;
		*(.data*)
		. = ALIGN(4);
		__data_end = .;
	} > RAM AT > ROM

	.bss : {
    __bss_start = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end = .;
	} > RAM

	. = ALIGN(4);
	__all_end = .;
}

PROVIDE(__stack = ORIGIN(RAM) + LENGTH(RAM));
