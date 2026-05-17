/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _DRIVERS_FIRMWARE_EFI_EFISTUB_H
#define _DRIVERS_FIRMWARE_EFI_EFISTUB_H

#include <linux/compiler.h>
#include <linux/efi.h>
#include <linux/types.h>

typedef union efi_dxe_services_table efi_dxe_services_table_t;
extern const efi_dxe_services_table_t *efi_dxe_table;

efi_status_t __efiapi efi_pe_entry(efi_handle_t handle,
				   efi_system_table_t *sys_table_arg);

#define efi_is_native()			(true)
#define efi_table_attr(inst, attr)	(inst)->attr
#define efi_fn_call(inst, func, ...)	(inst)->func(__VA_ARGS__)

#define efi_call_proto(inst, func, ...) ({			\
	__typeof__(inst) __inst = (inst);			\
	efi_fn_call(__inst, func, __inst, ##__VA_ARGS__);	\
})
#define efi_bs_call(func, ...) \
	efi_fn_call(efi_table_attr(efi_system_table, boottime), func, ##__VA_ARGS__)
#define efi_rt_call(func, ...) \
	efi_fn_call(efi_table_attr(efi_system_table, runtime), func, ##__VA_ARGS__)
#define efi_dxe_call(func, ...) \
	efi_fn_call(efi_dxe_table, func, ##__VA_ARGS__)

#define get_efi_var(name, vendor, ...)				\
	efi_rt_call(get_variable, (efi_char16_t *)(name),	\
		    (efi_guid_t *)(vendor), __VA_ARGS__)

#define set_efi_var(name, vendor, ...)				\
	efi_rt_call(set_variable, (efi_char16_t *)(name),	\
		    (efi_guid_t *)(vendor), __VA_ARGS__)

#define efi_get_handle_at(array, idx)					\
	(efi_is_native() ? (array)[idx] 				\
		: (efi_handle_t)(unsigned long)((u32 *)(array))[idx])

#define efi_get_handle_num(size)					\
	((size) / (efi_is_native() ? sizeof(efi_handle_t) : sizeof(u32)))

#define for_each_efi_handle(handle, array, num)				\
	for (int __i = 0; __i < (num) &&				\
		((handle = efi_get_handle_at((array), __i)) || true);	\
	     __i++)

typedef struct efi_generic_dev_path efi_device_path_protocol_t;

typedef void *efi_event_t;
/* Note that notifications won't work in mixed mode */
typedef void (__efiapi *efi_event_notify_t)(efi_event_t, void *);

typedef enum {
	EfiTimerCancel,
	EfiTimerPeriodic,
	EfiTimerRelative
} EFI_TIMER_DELAY;

/*
 * EFI Boot Services table
 */
union efi_boot_services {
	struct {
		efi_table_hdr_t hdr;
		void *raise_tpl;
		void *restore_tpl;
		efi_status_t (__efiapi *allocate_pages)(int, int, unsigned long,
							efi_physical_addr_t *);
		efi_status_t (__efiapi *free_pages)(efi_physical_addr_t,
						    unsigned long);
		efi_status_t (__efiapi *get_memory_map)(unsigned long *, void *,
							unsigned long *,
							unsigned long *, u32 *);
		efi_status_t (__efiapi *allocate_pool)(int, unsigned long,
						       void **);
		efi_status_t (__efiapi *free_pool)(void *);
		efi_status_t (__efiapi *create_event)(u32, unsigned long,
						      efi_event_notify_t, void *,
						      efi_event_t *);
		efi_status_t (__efiapi *set_timer)(efi_event_t,
						  EFI_TIMER_DELAY, u64);
		efi_status_t (__efiapi *wait_for_event)(unsigned long,
							efi_event_t *,
							unsigned long *);
		void *signal_event;
		efi_status_t (__efiapi *close_event)(efi_event_t);
		void *check_event;
		void *install_protocol_interface;
		void *reinstall_protocol_interface;
		void *uninstall_protocol_interface;
		efi_status_t (__efiapi *handle_protocol)(efi_handle_t,
							 efi_guid_t *, void **);
		void *__reserved;
		void *register_protocol_notify;
		efi_status_t (__efiapi *locate_handle)(int, efi_guid_t *,
						       void *, unsigned long *,
						       efi_handle_t *);
		efi_status_t (__efiapi *locate_device_path)(efi_guid_t *,
							    efi_device_path_protocol_t **,
							    efi_handle_t *);
		efi_status_t (__efiapi *install_configuration_table)(efi_guid_t *,
								     void *);
		efi_status_t (__efiapi *load_image)(bool, efi_handle_t,
						    efi_device_path_protocol_t *,
						    void *, unsigned long,
						    efi_handle_t *);
		efi_status_t (__efiapi *start_image)(efi_handle_t, unsigned long *,
						     efi_char16_t **);
		efi_status_t __noreturn (__efiapi *exit)(efi_handle_t,
							 efi_status_t,
							 unsigned long,
							 efi_char16_t *);
		efi_status_t (__efiapi *unload_image)(efi_handle_t);
		efi_status_t (__efiapi *exit_boot_services)(efi_handle_t,
							    unsigned long);
		void *get_next_monotonic_count;
		efi_status_t (__efiapi *stall)(unsigned long);
		void *set_watchdog_timer;
		void *connect_controller;
		efi_status_t (__efiapi *disconnect_controller)(efi_handle_t,
							       efi_handle_t,
							       efi_handle_t);
		void *open_protocol;
		void *close_protocol;
		void *open_protocol_information;
		void *protocols_per_handle;
		efi_status_t (__efiapi *locate_handle_buffer)(int, efi_guid_t *,
							      void *, unsigned long *,
							      efi_handle_t **);
		efi_status_t (__efiapi *locate_protocol)(efi_guid_t *, void *,
							 void **);
		efi_status_t (__efiapi *install_multiple_protocol_interfaces)(efi_handle_t *, ...);
		efi_status_t (__efiapi *uninstall_multiple_protocol_interfaces)(efi_handle_t, ...);
		void *calculate_crc32;
		void (__efiapi *copy_mem)(void *, const void *, unsigned long);
		void (__efiapi *set_mem)(void *, unsigned long, unsigned char);
		void *create_event_ex;
	};
};

typedef enum {
	EfiGcdMemoryTypeNonExistent,
	EfiGcdMemoryTypeReserved,
	EfiGcdMemoryTypeSystemMemory,
	EfiGcdMemoryTypeMemoryMappedIo,
	EfiGcdMemoryTypePersistent,
	EfiGcdMemoryTypeMoreReliable,
	EfiGcdMemoryTypeMaximum
} efi_gcd_memory_type_t;

typedef struct {
	efi_physical_addr_t base_address;
	u64 length;
	u64 capabilities;
	u64 attributes;
	efi_gcd_memory_type_t gcd_memory_type;
	void *image_handle;
	void *device_handle;
} efi_gcd_memory_space_desc_t;

/*
 * EFI DXE Services table
 */
union efi_dxe_services_table {
	struct {
		efi_table_hdr_t hdr;
		void *add_memory_space;
		void *allocate_memory_space;
		void *free_memory_space;
		void *remove_memory_space;
		efi_status_t (__efiapi *get_memory_space_descriptor)(efi_physical_addr_t,
								     efi_gcd_memory_space_desc_t *);
		efi_status_t (__efiapi *set_memory_space_attributes)(efi_physical_addr_t,
								     u64, u64);
		void *get_memory_space_map;
		void *add_io_space;
		void *allocate_io_space;
		void *free_io_space;
		void *remove_io_space;
		void *get_io_space_descriptor;
		void *get_io_space_map;
		void *dispatch;
		void *schedule;
		void *trust;
		void *process_firmware_volume;
		void *set_memory_space_capabilities;
	};
};

typedef union efi_memory_attribute_protocol efi_memory_attribute_protocol_t;

union efi_memory_attribute_protocol {
	struct {
		efi_status_t (__efiapi *get_memory_attributes)(
			efi_memory_attribute_protocol_t *, efi_physical_addr_t, u64, u64 *);

		efi_status_t (__efiapi *set_memory_attributes)(
			efi_memory_attribute_protocol_t *, efi_physical_addr_t, u64, u64);

		efi_status_t (__efiapi *clear_memory_attributes)(
			efi_memory_attribute_protocol_t *, efi_physical_addr_t, u64, u64);
	};
};

typedef struct {
	u16 scan_code;
	efi_char16_t unicode_char;
} efi_input_key_t;

union efi_simple_text_input_protocol {
	struct {
		void *reset;
		efi_status_t (__efiapi *read_keystroke)(efi_simple_text_input_protocol_t *,
							efi_input_key_t *);
		efi_event_t wait_for_key;
	};
};

union efi_simple_text_output_protocol {
	struct {
		void *reset;
		efi_status_t (__efiapi *output_string)(efi_simple_text_output_protocol_t *,
						       efi_char16_t *);
		void *test_string;
	};
};

#endif
