#include "io.h"
#include "direct_io.h"
#include "renderer.h"
#include <stdarg.h>

int kprintf(const char *format) {

  print_DIRECT(&__gRenderer, format);

  return 0;
}
