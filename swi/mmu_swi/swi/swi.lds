ENTRY(_start)

SECTIONS {
    .init  0x0	:   
    {
     crt0/crt0.o;
     *(.init) 
     }
    .text 0xb0004000 :AT(2048)
    {
    	*(.text)
    }
    .rodata :
    {
    	*(.rodata)
    }    
    .data :
    {
    	*(.data)
    }
    .bss :
    {
    	*(.bss) *(.COMM)
    }
}
