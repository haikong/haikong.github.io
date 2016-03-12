ENTRY(_start)

SECTIONS {
    .init  0x0	:AT(0)   
    {
     crt0/crt0.o;
     nand/built-in.o;
     *(.init)
     }
     . = ALIGN(4);
    .text 0x30000000 :AT(4096)
    {
    	_text_start  = .;
    	*(.text);
    	_text_end    = (. + 3) / 4;
    	_text_size   = _text_end - _text_start;    	
    }
    . = ALIGN(4);
    .rodata :
    {
    	*(.rodata)
    }   
    . = ALIGN(4); 
    .data :
    {
    	*(.data)
    }
    . = ALIGN(4);
    _bss_start = .;
    .bss :
    {
    	*(.bss) *(.COMM)
    }
    _bss_end = .;
}
