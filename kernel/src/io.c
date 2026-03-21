#include "io.h"
#include "direct_io.h"
#include "cstr.h"
#include "renderer.h"
#include <stdarg.h>

int kprintf(const char *format, ...) {

  va_list args;
  va_start(args, format);

  while(*format) {
    if(*format == '%') {
      format++;  
      
      if(*format == 's') {
        char *str = va_arg(args, char*);
        print_DIRECT(&__gRenderer, str);
      }

      if(*format == 'd') {
        int num = va_arg(args, int);
        print_DIRECT(&__gRenderer, int_to_string(num));
      }

      if(*format == 'f') {
        double num = va_arg(args, double);
        print_DIRECT(&__gRenderer, double_to_string(num));
      }

      if(*format == 'x') {
        int num = va_arg(args, int);
        print_DIRECT(&__gRenderer, hex_to_string(num));
      }

    } else {
      char str[2] = {*format, '\0'};
      print_DIRECT(&__gRenderer, str);
    }

    format++;
  }

  return 0;
}
