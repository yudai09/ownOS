SECTIONS{
    . = 0x40000000;
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
    PROVIDE(end = .);
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
        crt0.o(.text)   
        *(.text)
    }

}

