#!/bin/bash

CPPFLAGS=(-nostdinc -I./arch/x86/include -I./include -include ./include/linux/kconfig.h -include ./include/linux/compiler_types.h -D__KERNEL__)

CFLAGS=(-Wno-unused-parameter -g3)
LDFLAGS=(-O2 --orphan-handling=error)

set -x
set -e
clang-me "${CPPFLAGS[@]}" "${CFLAGS[@]}" -D__ASSEMBLY__ arch/x86/boot/efi/efi_pe_header.S -c -o arch/x86/boot/efi/efi_pe_header.o
clang-me "${CPPFLAGS[@]}" "${CFLAGS[@]}" -fshort-wchar -fpie arch/x86/boot/efi/entry.c -c -o arch/x86/boot/efi/entry.o
clang-me "${CPPFLAGS[@]}" "${CFLAGS[@]}" -D__ASSEMBLY__ arch/x86/boot/efi/efi_pe.lds.S -E -o arch/x86/boot/efi/efi_pe.lds
#objcopy --set-section-flags .rodata.str2.2=alloc --set-section-flags .rodata.cst16=alloc arch/x86/boot/efi/entry.o arch/x86/boot/efi/entry.o

if false; then
	ld.lld "${LDFLAGS[@]}" -pie --no-dynamic-linker -T arch/x86/boot/efi/efi_pe.lds arch/x86/boot/efi/efi_pe_header.o arch/x86/boot/efi/entry.o -o arch/x86/boot/efi/efi_pe.elf
	llvm-objcopy -O binary -j .text arch/x86/boot/efi/efi_pe.elf arch/x86/boot/efi/efi_pe_text.bin
	llvm-objcopy -O binary -j .data arch/x86/boot/efi/efi_pe.elf arch/x86/boot/efi/efi_pe_data.bin
	llvm-objcopy -O binary -j .efi_pe_header arch/x86/boot/efi/efi_pe.elf arch/x86/boot/efi/efi_pe_header.bin
else
	ld "${LDFLAGS[@]}" -pie --no-dynamic-linker -T arch/x86/boot/efi/efi_pe.lds arch/x86/boot/efi/efi_pe_header.o arch/x86/boot/efi/entry.o -o arch/x86/boot/efi/efi_pe.elf
	objcopy -O binary -j .text arch/x86/boot/efi/efi_pe.elf arch/x86/boot/efi/efi_pe_text.bin
objcopy -O binary -j .data arch/x86/boot/efi/efi_pe.elf arch/x86/boot/efi/efi_pe_data.bin
objcopy -O binary -j .efi_pe_header arch/x86/boot/efi/efi_pe.elf arch/x86/boot/efi/efi_pe_header.bin
fi

(cat arch/x86/boot/efi/efi_pe_header.bin;
dd if=arch/x86/boot/efi/efi_pe_text.bin bs=512 conv=sync status=none;
dd if=arch/x86/boot/efi/efi_pe_data.bin bs=512 conv=sync status=none) > arch/x86/boot/bootx64.efi
