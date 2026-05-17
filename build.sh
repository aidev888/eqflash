#!/bin/bash

CPPFLAGS=(-nostdinc -I./arch/x86/include -I./include -include ./include/linux/kconfig.h -include ./include/linux/compiler_types.h -D__KERNEL__)

CFLAGS=(-Wno-unused-parameter)

set -x
set -e
clang-me "${CPPFLAGS[@]}" "${CFLAGS[@]}" -D__ASSEMBLY__ arch/x86/boot/header.S -c -o arch/x86/boot/header.o
clang-me "${CPPFLAGS[@]}" "${CFLAGS[@]}" -fshort-wchar -fpie arch/x86/boot/text.c -c -o arch/x86/boot/text.o
clang-me "${CPPFLAGS[@]}" "${CFLAGS[@]}" -D__ASSEMBLY__ arch/x86/boot/setup.lds.S -E -o arch/x86/boot/setup.lds

ld.lld -pie --no-dynamic-linker -T arch/x86/boot/setup.lds arch/x86/boot/text.o arch/x86/boot/header.o -o arch/x86/boot/setup.elf
llvm-objcopy -O binary -j .text arch/x86/boot/setup.elf arch/x86/boot/setup_text.bin
llvm-objcopy -O binary -j .data arch/x86/boot/setup.elf arch/x86/boot/setup_data.bin
llvm-objcopy -O binary -j .efi_header arch/x86/boot/setup.elf arch/x86/boot/efi_header.bin

(dd if=arch/x86/boot/efi_header.bin bs=512 conv=sync status=none;
dd if=arch/x86/boot/setup_text.bin bs=512 conv=sync status=none;
dd if=arch/x86/boot/setup_data.bin bs=512 conv=sync status=none) > arch/x86/boot/bootx64.efi
