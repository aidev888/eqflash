#!/bin/bash

CPPFLAGS=(-nostdinc -I./arch/x86/include -I./include -include ./include/linux/kconfig.h -include ./include/linux/compiler_types.h -D__KERNEL__)

CFLAGS=(-Wno-unused-parameter)

set -x
set -e
clang-me "${CPPFLAGS[@]}" "${CFLAGS[@]}" -D__ASSEMBLY__ arch/x86/boot/efi/efi_pe_header.S -c -o arch/x86/boot/efi/efi_pe_header.o
clang-me "${CPPFLAGS[@]}" "${CFLAGS[@]}" -fshort-wchar -fpie arch/x86/boot/efi/entry.c -c -o arch/x86/boot/efi/entry.o
clang-me "${CPPFLAGS[@]}" "${CFLAGS[@]}" -D__ASSEMBLY__ arch/x86/boot/efi/efi_pe.lds.S -E -o arch/x86/boot/efi/efi_pe.lds

ld.lld -pie --no-dynamic-linker -T arch/x86/boot/efi/efi_pe.lds arch/x86/boot/efi/efi_pe_header.o arch/x86/boot/efi/entry.o -o arch/x86/boot/efi/efi_pe.elf
llvm-objcopy -O binary -j .text arch/x86/boot/efi/efi_pe.elf arch/x86/boot/efi/efi_pe_text.bin
llvm-objcopy -O binary -j .data arch/x86/boot/efi/efi_pe.elf arch/x86/boot/efi/efi_pe_data.bin
llvm-objcopy -O binary -j .efi_pe_header arch/x86/boot/efi/efi_pe.elf arch/x86/boot/efi/efi_pe_header.bin

(cat arch/x86/boot/efi/efi_pe_header.bin;
dd if=arch/x86/boot/efi/efi_pe_text.bin bs=512 conv=sync status=none;
dd if=arch/x86/boot/efi/efi_pe_data.bin bs=512 conv=sync status=none) > arch/x86/boot/bootx64.efi
