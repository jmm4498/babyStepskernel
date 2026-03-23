#include "cstr.h"
#include "defs.h"
#include "direct_io.h"
#include "efi_types.h"
#include "io.h"
#include "kernel_lib.h"
#include "renderer.h"

void _start(Framebuffer *framebuffer, PSF1_FONT *psf1_font,
            EFI_MEMORY_DESCRIPTOR *map, UINTN map_size, UINTN descriptor_size,
            UINT32 descriptor_version) {

  serial_init();

  serial_print("Kernel started\n");

  __renderer_init(&__gRenderer, framebuffer, psf1_font, OS_COLOR_STD);

  __renderer_set_cursor(&__gRenderer, 45, 0);

  UINTN entry_count = map_size / descriptor_size;

  serial_printf("\n\n:::Memory Map:::\n\n");
  serial_printf("Memory Map Entries: ");
  serial_printf("%x\n", entry_count);

  for (UINTN i = 0; i < entry_count; i++) {

    EFI_MEMORY_DESCRIPTOR *desc =
        (EFI_MEMORY_DESCRIPTOR *)(((UINT8 *)map) + (i * descriptor_size));

    serial_printf("Entry %x: Type=%x, Phys=0x%x, Pages=0x%x\n", i, desc->Type,
                  desc->PhysicalStart, desc->NumberOfPages);

    // We can only uncomment this once we have some more safety on the
    // framebuffer right now, this loop will blow through the memory and cause a
    // CPU exception
    // kprintf("Entry %x: Type= %x, Phys=0x%x, Pages=0x%x\n", i, desc->Type,
    //         desc->PhysicalStart, desc->NumberOfPages);
  }

  // no op to stop before handed back to UEFI
  kprintf("%s\n", "Kernel finished execution");

  while (1) {
    ;
  }

  return;
}
