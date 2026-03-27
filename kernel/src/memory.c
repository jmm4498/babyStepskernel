#include "memory.h"
#include "efi_types.h"

MEMORY_MAP _map = {0};

void add_range(UINT64 physical_start, UINT64 pages);

const char *memory_type_name(UINT32 type) {

  switch (type) {
  case 0:
    return "RESERVED       ";
  case 1:
    return "LOADERCODE     ";
  case 2:
    return "LOADERDATA     ";
  case 3:
    return "BOOTSERVICECODE";
  case 4:
    return "BOOTSERVICEDATA";
  case 5:
    return "RUNTIME CODE   ";
  case 6:
    return "RUNTIME DATA   ";
  case 7:
    return "CONVENTIONAL   ";
  case 8:
    return "UNUSABLE       ";
  case 9:
    return "ACPIRECLAIM    ";
  case 10:
    return "ACPINONVOLATILE";
  default:
    return "UNKNOWN        ";
  }
}

void init_physical_memory_map(EFI_MEMORY_DESCRIPTOR *map, UINTN map_size,
                              UINTN descriptor_size,
                              UINT32 descriptor_version) {

  _map.entry_count = map_size / descriptor_size;
  _map.descriptor = map;
  _map.descriptor_size = descriptor_size;
  _map.descriptor_version = descriptor_version;

  for (UINTN i = 0; i < _map.entry_count; i++) {

    EFI_MEMORY_DESCRIPTOR *desc =
        (EFI_MEMORY_DESCRIPTOR *)(((UINT8 *)_map.descriptor) +
                                  (i * _map.descriptor_size));

    // if ((desc->Type == 7 || desc->Type == 1 || desc->Type == 2 ||
    //      desc->Type == 3 || desc->Type == 4) &&
    //     _map.free_range_count < 256) {
    if(desc->Type == 7 && _map.free_range_count < 256) {
      add_range(desc->PhysicalStart, desc->NumberOfPages);
    }
  }

  return;
}

// This probably sucks
MEMORY_MAP *get_mmap() { return &_map; }

void add_range(UINT64 physical_start, UINT64 pages) {

  _map.free_ranges[_map.free_range_count].physical_start = physical_start;
  _map.free_ranges[_map.free_range_count].pages = pages;

  _map.free_range_count++;

  return;
}


void zero_page(uint64_t physical_address) {
  __memset((void*)physical_address, 0, PAGE_SIZE);
}
