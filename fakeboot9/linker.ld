OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(main)

SECTIONS {
	. = 0xFFFF0000;
	.boot9 : {
	    boot9.bin.o(.rodata.boot9_bin)
	}

	. += 0xFFFF8000 - (ADDR(.boot9) + SIZEOF(.boot9));

	.text : {
    *(.start)
    *(entry)
    *(.init)
    *(.text)
    *(.ctors)
    *(.dtors)
    *(.fini)
	}

	. += 0xFFFF9368 - (ADDR(.text) + SIZEOF(.text));

	.reset : {
    reset.o(.reset)
	}    
	    
	.rodata : {
    main.o(.rodata)
	}

  /DISCARD/ :
  {
      *(*);
  }

    
  . = ALIGN(4);
}