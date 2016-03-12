ENTRY(_start)

SECTIONS {
    .init  0x0	:AT(0)   
    {
     crt0/crt0.o;
     nand/built-in.o;
     *(.init)
     }
    .text 0x30000000 :AT(4096)
    {
    	_text_start  = .;
    	*(.text);
    	_text_end    = (. + 3) / 4;
    	_text_size   = _text_end - _text_start;    	
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
