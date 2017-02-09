MEMORY
{
    FLASH (rx) : ORIGIN = 0x0, LENGTH = 1M
    SRAM (rwx) : ORIGIN = 0x10000000, LENGTH = 32k
	ABHSRAM (rwx) : ORIGIN = 0x2007c000, LENGTH = 32k
}

ENTRY(main)

SECTIONS
{
  .isr_vectors : {
	*(.isr_vectors)
	. = ALIGN(4);
	PROVIDE(_DATA_FLASH_START = .);
  } >FLASH

  .text : {
	*(.text*)
	*(.rodata*)
	. = ALIGN(4);
  } >FLASH

  .stackarea : {
	*(.stackarea)
  } >SRAM

  .data : {
    . = ALIGN(4);
    PROVIDE(_DATA_START = .);
    *(.data)
    PROVIDE(_DATA_END = .);

  } >SRAM AT>FLASH

  .bss : {
    . = ALIGN(4);
    PROVIDE(__bss_start__ = .);
    *(.bss)
    PROVIDE(__bss_end__ = .);
  } >SRAM

  PROVIDE(__HEAP_START = __bss_end__);

  /* DWARF debug sections.
   * Symbols in the DWARF debugging sections are relative to
   * the beginning of the section so we begin them at 0.
   */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* DWARF 2.1 */
    /* DWARF 2.1 */
  .debug_ranges   0 : { *(.debug_ranges) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }

  .note.gnu.arm.ident 0 : { KEEP (*(.note.gnu.arm.ident)) }
  .ARM.attributes 0 : { KEEP (*(.ARM.attributes)) }
  /DISCARD/ : { *(.note.GNU-stack) }
}

