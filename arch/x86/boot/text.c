#include <linux/efi.h>

#include "efistub.h"

const efi_system_table_t *efi_system_table;

static void efi_char16_puts(efi_char16_t *str)
{
	efi_call_proto(efi_table_attr(efi_system_table, con_out),
		       output_string, str);
}

static void __noreturn efi_exit(efi_handle_t handle, efi_status_t status)
{
	efi_bs_call(exit, handle, status, 0, NULL);
	for(;;)
		asm("hlt");
}

const efi_dxe_services_table_t *efi_dxe_table;

void *get_efi_config_table(efi_guid_t guid)
{
	unsigned long tables = efi_table_attr(efi_system_table, tables);
	int nr_tables = efi_table_attr(efi_system_table, nr_tables);
	int i;

	for (i = 0; i < nr_tables; i++) {
		efi_config_table_t *t = (void *)tables;

		if (efi_guidcmp(t->guid, guid) == 0)
			return efi_table_attr(t, table);

		tables += efi_is_native() ? sizeof(efi_config_table_t)
					  : sizeof(efi_config_table_32_t);
	}
	return NULL;
}

static efi_memory_attribute_protocol_t *memattr;

efi_status_t __efiapi efi_pe_entry(efi_handle_t handle,
				   efi_system_table_t *sys_table_arg)
{
	efi_guid_t guid = EFI_MEMORY_ATTRIBUTE_PROTOCOL_GUID;

	efi_system_table = sys_table_arg;
	/* Check if we were booted by the EFI firmware */
	if (efi_system_table->hdr.signature != EFI_SYSTEM_TABLE_SIGNATURE)
		efi_exit(handle, EFI_INVALID_PARAMETER);

	efi_dxe_table = get_efi_config_table(EFI_DXE_SERVICES_TABLE_GUID);
	if (efi_dxe_table &&
	    efi_dxe_table->hdr.signature == EFI_DXE_SERVICES_TABLE_SIGNATURE) {
		efi_char16_puts(L"system support efi_dxe_table!\n");
	} else {
		efi_char16_puts(L"system not support efi_dxe_table!\n");
	}


	efi_bs_call(locate_protocol, &guid, NULL, (void **)&memattr);
	if (memattr)
		efi_char16_puts(L"has memattr!\n");
	else
		efi_char16_puts(L"not has memattr!\n");


	efi_char16_puts(L"Hello World!\n");
	efi_exit(handle, 0);
}
