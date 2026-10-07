ENTRY(Start)

gNumMusicPlayers = 4;
gMaxLines = 0;
gInitialMainCB2 = CB2_InitCopyrightScreenAfterBootup;

MEMORY
{
    EWRAM (rwx) : ORIGIN = 0x2000000, LENGTH = 256K
    IWRAM (rwx) : ORIGIN = 0x3000000, LENGTH = 32K
    ROM    (rx) : ORIGIN = 0x8000000, LENGTH = 32M
}

SECTIONS {
    ewram 0x2000000 (NOLOAD) :
    ALIGN(4)
    {
        src/*.o(ewram_data);
        gflib/*.o(ewram_data);
        . = ALIGN(4);
    } > EWRAM

    iwram_code 0x3000000 : AT (gIwramCodeLma)
    ALIGN(4)
    {
        gIwramCodeStart = .;
        *.o(iwram_code*);
        . = ALIGN(4);
        . += 4; /* Ensure size is always > 0 */
    } > IWRAM
    gIwramCodeWords = SIZEOF(iwram_code) / 4;

    iwram (NOLOAD) :
    ALIGN(4)
    {
        *.o(.bss*);
        *.o(iwram_data);
        *.o(common_data);
        src/*.o(COMMON);
        *libc.a:*.o(COMMON);
        *libnosys.a:*.o(COMMON);
        . = ALIGN(4);
        __iwram_end = .;
    } > IWRAM

    /* BEGIN ROM DATA */
    . = 0x8000000;

    .text :
    ALIGN(4)
    {
        src/rom_header.o(.text*);
        KEEP(*.o(.text.header_gf));
        KEEP(*.o(.text.header_rhh));
        src/crt0.o(.text);
        *libagbsyscall.a:*.o(.text*);
        *libgcc.a:*.o(.text*);
        *libc.a:*.o(.text*);
        *libnosys.a:*.o(.text*);
        asm/*.o(.text*);
        KEEP(*.o(.text.consts));
        *.o(.text*);
        *.o(.eh_frame);
    } > ROM =0

    script_data :
    ALIGN(4)
    {
        data/*.o(script_data);
    } > ROM =0

    .rodata.compound_string :
    SUBALIGN(0)
    {
        *.o(.rodata.compound_string.*);
    } > ROM =0

    .rodata :
    ALIGN(4)
    {
        *.o(.rodata*);
        *libgcc.a:*.o(.rodata*);
        *libc.a:*.o(.rodata*);
        *libc.a:*.o(.data*);
        sound/songs/*.o(.rodata);
    } > ROM =0

    /* EWRAM/IWRAM code sections are listed here
       in order to have them have VMAs after their BSS siblings and have LMAs in ROM */
    /* See https://sourceware.org/binutils/docs/ld/Builtin-Functions.html */
    . = ALIGN(4);
    gEwramCodeLma = .;
    ewram_code : AT (gEwramCodeLma)
    ALIGN(4)
    {
        gEwramCodeStart = .;
        *.o(ewram_code*);
        . = ALIGN(4);
        . += 4; /* Ensure size is always > 0 */
    } > EWRAM
    gEwramCodeWords = SIZEOF(ewram_code) / 4;
    . = LOADADDR(ewram_code) + SIZEOF(ewram_code);

    . = ALIGN(4);
    gIwramCodeLma = .;
    . = LOADADDR(iwram_code) + SIZEOF(iwram_code);

    /* Note: Do not place any other ROM sections after this,
       unless they are structured like: */
    /*
    __after_iwram_code = .;
    custom_rom_section : AT(__after_iwram_code)
    ALIGN(4)
    {
        ...
    } > ROM
    . = LOADADDR(custom_rom_section) + SIZEOF(custom_rom_section);
    __after_custom_rom_section = .;
    */

    __rom_end = .;

    /* DWARF debug sections.
       Symbols in the DWARF debugging sections are relative to the beginning
       of the section so we begin them at 0.  */

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

    /* DWARF 5*/
    .debug_line_str     0 : { *(.debug_line_str) }

    /* Discard everything not specifically mentioned above. */
    /DISCARD/ :
    {
        *(*);
    }
}
