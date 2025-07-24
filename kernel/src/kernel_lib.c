#include "kernel_lib.h"



int __memcpy(void *src, void *dest, size_t n) {

  if(src == 0 || dest == 0 || n < 1) {
    return 1;
  }

  // Byte pointers for fine-grained control
  uint8_t *d = (uint8_t *)dest;
  const uint8_t *s = (const uint8_t *)src;

    // For small sizes (<16 bytes), use byte-wise copying to avoid overhead
  if (n < 16) {
    while (n--) {
      *d++ = *s++;
    }
    return 0;
  }

    // Check alignment (8-byte for x86-64)
    if (((uintptr_t)d & 7) == 0 && ((uintptr_t)s & 7) == 0) {
        // Both dest and src are 8-byte aligned, use movsq for bulk copy
        size_t n_quads = n / 8;
        size_t remainder = n % 8;

        // Inline assembly for movsq
        asm volatile (
            "mov %2, %%rcx;"    // Set count to n_quads
            "rep movsq;"        // Copy 8 bytes at a time
            : "+D"(d), "+S"(s)  // Update dest and src pointers
            : "r"(n_quads)
            : "rcx", "memory"
        );

        // Handle remaining bytes
        while (remainder--) {
            *d++ = *s++;
        }
    } else {
        // Unaligned: copy byte-by-byte until dest is aligned
        while (n && ((uintptr_t)d & 7)) {
            *d++ = *s++;
            n--;
        }

        // Now dest is aligned, check if src is aligned and n is large enough
        if (n >= 8 && ((uintptr_t)s & 7) == 0) {
            size_t n_quads = n / 8;
            size_t remainder = n % 8;

            asm volatile (
                "mov %2, %%rcx;"
                "rep movsq;"
                : "+D"(d), "+S"(s)
                : "r"(n_quads)
                : "rcx", "memory"
            );

            // Handle remaining bytes
            while (remainder--) {
                *d++ = *s++;
            }
        } else {
            // Src unaligned or small remainder, finish with byte-wise copy
            while (n--) {
                *d++ = *s++;
            }
        }
    }

  return 0;
}
