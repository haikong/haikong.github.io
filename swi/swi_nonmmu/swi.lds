ENTRY(_start)
MEMORY{
	rom(rx):ORIGIN = 0x30000000,LENGTH = 1M
}
SECTIONS {
    . = 0x30000000;
    . = ALIGN(4);
    .text          :   
    {
     crt0/crt0.o(.text);
     *(.text);
     }AT>rom
    .rodata ALIGN(4) : {*(.rodata)}AT>rom 
    .data ALIGN(4) : { *(.data) }AT>rom
    .bss ALIGN(4)  : { *(.bss)  *(COMMON) }
}
