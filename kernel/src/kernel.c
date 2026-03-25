#include "cstr.h"
#include "defs.h"
#include "direct_io.h"
#include "efi_types.h"
#include "io.h"
#include "kernel_lib.h"
#include "memory.h"
#include "renderer.h"

void _start(Framebuffer *framebuffer, PSF1_FONT *psf1_font,
            EFI_MEMORY_DESCRIPTOR *map, UINTN map_size, UINTN descriptor_size,
            UINT32 descriptor_version) {

  serial_init();

  serial_print("###############################################################"
               "################################\n");
  serial_print("Kernel started\n");

  __renderer_init(&__gRenderer, framebuffer, psf1_font, OS_COLOR_STD);

  __renderer_set_cursor(&__gRenderer, 45, 0);

  init_physical_memory_map(map, map_size, descriptor_size, descriptor_version);

  MEMORY_MAP *mmap = get_mmap();

  serial_printf("\n\n:::Memory Map:::\n\n");
  serial_printf("Memory Map Usable Entries: %x\n", mmap->free_range_count);

  // Lets print out how many usable pages we have
  for (UINTN i = 0; i < mmap->free_range_count; i++) {

    serial_printf("Entry %x: Phys=0x%x, Pages=0x%x\n", i,
                  mmap->free_ranges[i].physical_start,
                  mmap->free_ranges[i].pages);
  }

  // no op to stop before handed back to UEFI
  kprintf("%s\n", "Kernel finished execution");

  while (1) {
    ;
  }

  return;
}
