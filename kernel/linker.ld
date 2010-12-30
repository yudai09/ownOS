ENTRY (loader)

SECTIONS{
    . = 0x00100000;

	.text_loader :{
		loader.o(.text)
	}
    .rodata ALIGN (0x1000) : {
        *(.rodata)
    }

    .data ALIGN (0x1000) : {
        *(.data)
    }

    .bss : {
        sbss = .;
        *(COMMON)
        *(.bss)
        ebss = .;
    }
	.data ALIGN (0x1000) : {
   	start_ctors = .;
   	*(.ctor*)
   	end_ctors = .;
   	start_dtors = .;
   	*(.dtor*)
   	end_dtors = .;
   	*(.data)
	}
   .text :{
        *(.text)
    }
}

