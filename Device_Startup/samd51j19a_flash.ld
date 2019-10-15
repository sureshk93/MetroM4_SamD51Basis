/*
 Copyright 2019 June Hanabi

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)

/*
 * NVM = 2 Banks (512KB) (352KB usable) (160KB reserved for SEEPROM)
 * 1 Bank = 32 blocks (256K) and 16 Regions
 *    Bank 1 (Main) = 32 blocks (256K) and 16 Regions
 *    Bank 2 (Aux)  = 12 Blocks (96KB) and 6 Regions
 * 1 Region = 2 Blocks (16KB) (Protection Alignment)
 * 1 Block = 16 Pages (8KB) (Division Alignment)
 * 1 Page = 32 QWords (512B) (Section Alignment)
 * 1 QWord = 16 Bytes (Sub-Section Alignment)
 * 1 DWord = 8 Bytes
 * 1 Word  = 4 Bytes (Default Alignment)
 *
 * Alignments:
 * 4      Bytes: New piece
 * 16     Bytes: New Subsection
 * 512    Bytes: New Section
 * 8,192  Bytes: New division
 * 16,384 Bytes: New protection region
*/

/* Memory Spaces Definitions */
MEMORY
{
  rom0     (rx)  : ORIGIN = 0x00000000, LENGTH = 0x0003F800 /*Rom Bank 0: Main Bank (For Code)*/
    lnl      (rx)  : ORIGIN = 0x0003F800, LENGTH = 0x00000800 /*Load-N-Lock 0-Wait State Speed: 2KB*/

  rom1     (rx)  : ORIGIN = 0x00040000, LENGTH = 0x00018000 /*Rom Bank 1: Aux Bank (For Data)*/
  qspi     (rx)  : ORIGIN = 0x04000000, LENGTH = 0x01000000 /*External NVM: (Additional Aux Data on seperate chip)*/
  ram      (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00030000 /*Ram (Default RAM)*/
  seeprom  (rx)  : ORIGIN = 0x44000000, LENGTH = 0x00010000 /*SEEPROM: Runtime Permanent Data*/
  bkupram  (rwx) : ORIGIN = 0x47000000, LENGTH = 0x00002000 /*Backup Ram: Program Data*/
}

/* The stack size used by the application*/
STACK_SIZE = 0xC000;

/* Section Definitions */
SECTIONS
{
  /*
    Program Code

    Gameplan for .text output section

    No support for exceptions or unwinding tables

    Vectors go at start
    Program Code is placed on the next QWord
    Thumb/Arm Glue code is placed on the next QWord
    Read-Only Data is placed in it's own page
    Relocate Data is placed in it's own page
  */
  .text :
  {
    /*Start at top*/
    . = ALIGN(4); /*New piece*/
    _stext = .;
    _sfixed = .;

    /*Vector Table*/
    /*Place Vectors at start of rom0 as required*/
    KEEP(*(.vectors .vectors.*))

    /*Program Code*/
    /*. = ALIGN(16); New Subsection*/
    *(.text .text.* .gnu.linkonce.t.*)

    /*Thumb <==> ARM Code Translation Glue*/
    /*. = ALIGN(16); New Subsection*/
    *(.glue_7t) *(.glue_7)

    *(.ARM.extab* .gnu.linkonce.armextab.*)

    /*C & C++ Special Generated Code, part of main program code*/

    /*C++ Setup Code*/
    . = ALIGN(4); /*New piece*/
    KEEP(*(.init))

    . = ALIGN(4); /*New piece*/
    __preinit_array_start = .;
    KEEP (*(.preinit_array))
    __preinit_array_end = .;

    . = ALIGN(4); /*New piece*/
    __init_array_start = .;
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array))
    __init_array_end = .;

    /*C++ Constructor Code*/
    . = ALIGN(4); /*New piece*/
    KEEP (*crtbegin.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*crtend.o(.ctors))

    /*C++ Teardown Code*/
    . = ALIGN(4); /*New piece*/
    KEEP(*(.fini))

    . = ALIGN(4); /*New piece*/
    __fini_array_start = .;
    KEEP (*(.fini_array))
    KEEP (*(SORT(.fini_array.*)))
    __fini_array_end = .;

    /*C++ Deconstructor Code*/
    . = ALIGN(4); /*New piece*/
    KEEP (*crtbegin.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*crtend.o(.dtors))

    /*Read-Only/Constant Data, placing in a new page*/
    . = ALIGN(512); /*New Page*/
    *(.rodata .rodata* .gnu.linkonce.r.*)

    /*This is for relocating data, place it in it's own page*/
    . = ALIGN(512); /*New Page*/
    _placeRelocate = .;

    _efixed = .;
    _etext = .;
  } > rom0 /*These of course go in the main rom bank*/

  /*
    Rom 0 Custom Code

    Gameplan for .rom0 output section

    This is sort of a workarea for program-specific usage unrelated to generated
    code. It needs to go into it's own block.
  */
  .rom0 (NOLOAD):
  {
    . = ALIGN(8192); /*New block*/
    _srom0 = .;
    *(.rom0*)
    _erom0 = .;
  } > rom0

  /*
    Load-N-Lock 0-Wait State Speed

    Gameplan for .lnl output section

    LNL is placed in it's own reserved memory area of a specific size in rom0. Space is
    immensely cramped and every byte counts. No alignment will be done.
  */
  .lnl (NOLOAD):
  {
    . = ALIGN(4); /*New piece*/
    _slnl = .;
    *(.lnl*)
    _elnl = .;
  } > lnl /*Goes into it's own specific area*/

  /*
    Rom 1 Custom Code

    Gameplan for .rom1 output section

    Rom Bank 1 is more relaxed given the entire bank is for program usage.
    This needs to be placed after the first page. The first page is reserved
    for program-specific header data and work area.
  */
  .rom1 (NOLOAD):
  {
    . = 512; /*2nd page from start*/
    _srom1 = .;
    *(.rom1*)
    _erom1 = .;
  } > rom1 /*Goes into rom bank 1*/

  /*
    QSPI Custom Code

    Gameplan for .qspi output section

    QSPI is similar to ROM1 in that it's just an area that can be used
    for custom usage or work area.
  */
  .qspi (NOLOAD):
  {
    . = ALIGN(4); /*New piece*/
    _sqspi = .;
    *(.qspi*)
    _eqspi = .;
  } > qspi /*Goes into QSPI*/

  /*
    Uninitialized Data

    Gameplan for .bss output section

    Uninitialized data goes into main RAM at the start
  */
  .bss (NOLOAD) :
  {
    . = ALIGN(4); /*New piece*/
    _sbss = . ;
    _szero = .;
    *(.bss .bss.*) /*Place BSS Data at start of RAM*/

    . = ALIGN(4); /*New piece*/
    *(COMMON) /*Place common data after*/
    _ebss = . ;
    _ezero = .;

    /*Relocate data comes after bss*/
    . = ALIGN(4); /*New piece*/
    _relocateStart = .;
  } > ram

  /*
    Relocate data

    Gameplan for .relocate output section

    This needs to be placed at rom0 in it's own defined section. It will
    be copied to the end of bss.
  */
  .relocate : AT (_placeRelocate)
  {
    . = ALIGN(4); /*New piece*/
    _srelocate = .;
    *(.data .data.*);

    . = ALIGN(4); /*New Piece*/
    *(.ramfunc .ramfunc.*);
    _erelocate = .;
  } > ram /*Place in RAM*/

  /*
    Stack data

    Gameplan for .stack output section

    Placed at end of RAM an empty area large enough for the stack
  */
  .stack (NOLOAD):
  {
    . = ALIGN(8); /*New larger piece*/
    _sstack = .; /*Begin Stack*/

    . = . + STACK_SIZE; /*Insert large blank area for stack*/

    . = ALIGN(8); /*New larger piece*/
    _estack = .; /*End stack placement*/
  } > ram

  /*
    SEEPROM Data

    Gameplan for .seeprom output section

    The SEEPROM section is like rom1 just byte-writable rather than
    block/page eraseable/writable. Unlike ROM1 however SEEPROM can start
    at the beginning.
  */
  .seeprom (NOLOAD):
  {
    . = ALIGN(4); /*New piece*/
    _seeprom = .;
    *(.seeprom*)
    _eseeprom = .;
  } > seeprom /*Goes into seeprom area*/

  /*
    Custom Backup RAM

    Gameplan for .bkupram output section

    Like ROM1 and SEEPROM, this is another potential usage area.
  */
  .bkupram (NOLOAD):
  {
    . = ALIGN(4); /*New piece*/
    _sbkupram = .;
    *(.bkupram*)
    _ebkupram = .;
  } > bkupram /*Goes into bkupram area*/

  . = ALIGN(4); /*New piece*/
  _end = . ; /*End of everything*/
}
