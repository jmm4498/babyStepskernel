#include "renderer.h"
#include "cstr.h"
#include "defs.h"
#include "direct_io.h"
#include "io.h"
#include "kernel_lib.h"

void _start(Framebuffer *framebuffer, PSF1_FONT *psf1_font) {

  serial_init();

  serial_print("Kernel started\n");

  __renderer_init(&__gRenderer, framebuffer, psf1_font, OS_COLOR_STD);

  __renderer_set_cursor(&__gRenderer, 45, 0);

  kprintf("%s\n", "Hello from kprintf!");
  kprintf("%d\n", 12345);
  kprintf("%f\n", 3.14159);

  kprintf("1): %s 2): %d 3): %lf 4): %x\n", "string", 67, 2.184234, 0xFF);
  serial_printf("1): %s 2): %d 3): %lf 4): %x\n", "string", 67, 2.184234, 0xFF);

  //no op to stop before handed back to UEFI
  print_DIRECT(&__gRenderer, "Kernel finished execution\n");
  while(1) {
    ;
  }

  return;
}
