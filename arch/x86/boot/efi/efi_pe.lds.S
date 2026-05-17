#include "efi_pe.h"

OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64",
              "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(efi_pe_entry)

SECTIONS
{
	. = EFI_PE_SALIGN;

	.interp         : { *(.interp) }
	ASSERT(SIZEOF(.interp) == 0, "Unexpected interpreter detected, recompile with -static-pie -Wl,--no-dynamic-linker !")

	/* We put .rodata into .text */
	.text ALIGN(EFI_PE_SALIGN) : {
		*(.text.unlikely .text.*_unlikely .text.unlikely.*)
		*(.text.exit .text.exit.*)
		*(.text.startup .text.startup.*)
		*(.text.hot .text.hot.*)
		*(SORT(.text.sorted.*))
		*(.text .text.*)
		*(.rodata .rodata.*)
	}
	ASSERT(ADDR(.text) == EFI_PE_SALIGN, "Unexpected sections detected!")

	.data ALIGN(EFI_PE_SALIGN) : {
		*(.data .data.*)
	}

	.bss ALIGN(EFI_PE_SALIGN) : {
		*(.bss .bss.*)
		*(COMMON)
	}

	. = ALIGN(EFI_PE_SALIGN);
	PROVIDE(_efi_pe_virtual_end = .);

	.efi_pe_header : {
		KEEP(*(.efi_pe_header0))
		LONG(ALIGN(SIZEOF(.text), EFI_PE_FALIGN))
		LONG(ALIGN(SIZEOF(.data), EFI_PE_FALIGN))
		LONG(ALIGN(SIZEOF(.bss), EFI_PE_FALIGN))
		LONG(efi_pe_entry)
		LONG(ADDR(.text))
		KEEP(*(.efi_pe_header1))
		LONG(_efi_pe_virtual_end)
		KEEP(*(.efi_pe_header2))
		LONG(SIZEOF(.text))
		LONG(ADDR(.text))
		LONG(ALIGN(SIZEOF(.text), EFI_PE_FALIGN))
		KEEP(*(.efi_pe_header3))
		LONG(SIZEOF(.data))
		LONG(ADDR(.data))
		LONG(ALIGN(SIZEOF(.data), EFI_PE_FALIGN))
		LONG(EFI_PE_HEADER_SIZE + ALIGN(SIZEOF(.text), EFI_PE_FALIGN))
		KEEP(*(.efi_pe_header4))
		LONG(SIZEOF(.bss))
		LONG(ADDR(.bss))
		KEEP(*(.efi_pe_header5))
		. = ALIGN(EFI_PE_FALIGN);
	}
	ASSERT(SIZEOF(.efi_pe_header) == EFI_PE_HEADER_SIZE, "efi pe header too large!")

	.note.gnu.build-id  : { *(.note.gnu.build-id) }
	.hash           : { *(.hash) }
	.gnu.hash       : { *(.gnu.hash) }
	.dynsym         : { *(.dynsym) }
	.dynstr         : { *(.dynstr) }
	.eh_frame       : ONLY_IF_RO { KEEP (*(.eh_frame)) *(.eh_frame.*) }
	.eh_frame       : ONLY_IF_RW { KEEP (*(.eh_frame)) *(.eh_frame.*) }
	.dynamic        : { *(.dynamic) }

	.got.plt : { *(.got.plt) *(.igot.plt) }
	ASSERT(SIZEOF(.got.plt) == 0, "Unexpected GOT/PLT entries detected!")
	.got : { *(.got) *(.igot) }
	ASSERT(SIZEOF(.got) == 0, "Unexpected GOT entries detected!")
	.plt : { *(.plt) *(.plt.*) *(.iplt) }
	ASSERT(SIZEOF(.plt) == 0, "Unexpected run-time procedure linkages detected!")
	.rel.dyn : { *(.rel.*) *(.rel_*) }
	ASSERT(SIZEOF(.rel.dyn) == 0, "Unexpected run-time relocations (.rel) detected!")
	.rela.dyn : { *(.rela.*) *(.rela_*) }
	ASSERT(SIZEOF(.rela.dyn) == 0, "Unexpected run-time relocations (.rela) detected!")

	/* Stabs debugging sections.  */
	.stab          0 : { *(.stab) }
	.stabstr       0 : { *(.stabstr) }
	.stab.excl     0 : { *(.stab.excl) }
	.stab.exclstr  0 : { *(.stab.exclstr) }
	.stab.index    0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment       0 : { *(.comment) }
	.gnu.build.attributes : { *(.gnu.build.attributes .gnu.build.attributes.*) }
	/* DWARF debug sections.
	   Symbols in the DWARF debugging sections are relative to the beginning
	   of the section so we begin them at 0.  */
	/* DWARF 1.  */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }
	/* GNU DWARF 1 extensions.  */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2.  */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2.  */
	.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line .debug_line.* .debug_line_end) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions.  */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
	/* DWARF 3.  */
	.debug_pubtypes 0 : { *(.debug_pubtypes) }
	.debug_ranges   0 : { *(.debug_ranges) }
	/* DWARF 5.  */
	.debug_addr     0 : { *(.debug_addr) }
	.debug_line_str 0 : { *(.debug_line_str) }
	.debug_loclists 0 : { *(.debug_loclists) }
	.debug_macro    0 : { *(.debug_macro) }
	.debug_names    0 : { *(.debug_names) }
	.debug_rnglists 0 : { *(.debug_rnglists) }
	.debug_str_offsets 0 : { *(.debug_str_offsets) }
	.debug_sup      0 : { *(.debug_sup) }
	.gnu.attributes 0 : { KEEP (*(.gnu.attributes)) }

	/DISCARD/ : { *(.note.GNU-stack) *(.gnu_debuglink) *(.gnu.lto_*) }
}
