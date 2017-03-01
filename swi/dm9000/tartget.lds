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
    }
    _text_end    = (. + 3) & (0xfffffffc);
    .rodata ALIGN(4):
    {
    	*(.rodata);
    }    
    .data ALIGN(4):
    {
    	*(.data);
    }
    .init_data ALIGN(4):
    {
    	*(.init_data);
    }
    _bss_start = .;
    _text_size = _bss_start - _text_start;
    . = ALIGN(4);
    .bss :
    {
    	*(.bss) *(.COMM)
    }
    _bss_end = (. + 3) & (0xfffffffc);
}
