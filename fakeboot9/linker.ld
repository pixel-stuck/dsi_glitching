OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(main)

SECTIONS {
	. = 0xFFFF0000;
	.boot9 : {
	    boot9.bin.o(.rodata.boot9_bin)
	}

	. += 0xFFFF8000 - (ADDR(.boot9) + SIZEOF(.boot9));


	__code_start = . ;
	.text : {
    *(.start)
    *(entry)
    *(.init)
    *(.text)
    *(.ctors)
    *(.dtors)
    *(.fini)
	}

	. += 0xFFFF87F4 - (ADDR(.text) + SIZEOF(.text));

	.unkRsaData : {    
    Unk_Rsa_data.bin.o(.rodata.Unk_Rsa_data_bin)
	}

	. += 0xFFFF9368 - (ADDR(.unkRsaData) + SIZEOF(.unkRsaData));

	.reset : {
    reset.o(.reset)
	}    

	. += 0xFFFF9920 - (ADDR(.reset) + SIZEOF(.reset));    

	.keydata : {
    Unk_Rsa_Key.bin.o(.rodata.Unk_Rsa_Key_bin)
    NDS_Blowfish.bin.o(.rodata.NDS_Blowfish_bin)
	}

	. += 0xFFFFBA30 - (ADDR(.keydata) + SIZEOF(.keydata));    
	    
	.rodata : {
    main.o(.rodata)
	}

  __bss_start = . ;
	.bss : {
	    main.o(.bss COMMON)
	}
	__bss_end = . ;
	_end = __bss_end ;
	__end__ = __bss_end ;

  /DISCARD/ :
  {
      *(*);
  }

    
    . = ALIGN(4);
}