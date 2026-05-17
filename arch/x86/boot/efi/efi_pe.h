#pragma once

#define EFI_PE_HEADER_SIZE	0x200 /* EFI File pe header size */
#define EFI_PE_FALIGN		0x200 /* EFI File pe file alignment  */
#define EFI_PE_SALIGN		0x1000 /* EFI File pe section alignment */
#define EFI_PE_SECTIONS_NUM	3 /* .text .data .bss */

#define IS_POWER_OF_2(n)		(n != 0 && ((n & (n - 1)) == 0))

#if !IS_POWER_OF_2(EFI_PE_FALIGN)		|| \
	EFI_PE_FALIGN < 0x200			|| \
	!IS_POWER_OF_2(EFI_PE_SALIGN)		|| \
	EFI_PE_SALIGN < 0x1000			|| \
	EFI_PE_SALIGN < EFI_PE_FALIGN		|| \
	EFI_PE_HEADER_SIZE % EFI_PE_FALIGN != 0
#error "EFI Size check failed"
#endif
