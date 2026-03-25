#ifndef HH_MEMORY_HH
#define HH_MEMORY_HH

#include "efi_types.h"

// This stores the physical starting point of a given range of 4k pages
typedef struct memory_range {

  UINT64 physical_start;
  UINT64 pages;

} MEMORY_RANGE;

// the entire memory map, plus a subset that is usable memory ranges
typedef struct memory_map {

  UINT32 descriptor_version;
  UINTN descriptor_size;
  UINTN entry_count;
  EFI_MEMORY_DESCRIPTOR *descriptor;
  MEMORY_RANGE free_ranges[256];
  UINTN free_range_count;
} MEMORY_MAP;

const char *memory_type_name(UINT32 type);

void init_physical_memory_map(EFI_MEMORY_DESCRIPTOR *map, UINTN map_size,
                              UINTN descriptor_size, UINT32 descriptor_version);

MEMORY_MAP *get_mmap();

#endif
