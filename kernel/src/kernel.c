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

  const char *test = "Hello World!\n";

  print_DIRECT(&__gRenderer, test);

  char safe[32];
  __memcpy("Hello from stack!\n", safe, 19);
  safe[19] = '\0';

  print_DIRECT(&__gRenderer, safe);

  kprintf(safe);

  print_DIRECT(&__gRenderer, safe);

  return;
}
