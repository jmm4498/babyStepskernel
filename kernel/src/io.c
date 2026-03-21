#include "io.h"
#include "direct_io.h"
#include "cstr.h"
#include "renderer.h"
#include <stdarg.h>

//Simple printf implementations the print to display

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

      if(*format == 'l') {
        format++;
        if(*format == 'f') {
          double num = va_arg(args, double);
          print_DIRECT(&__gRenderer, double_to_string_precise(num, 6));
        }

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

//simple printf implementation that prints to serial port 1
int serial_printf(const char *format, ...) {

  va_list args;
  va_start(args, format);

  while(*format) {
    if(*format == '%') {
      format++;  
      
      if(*format == 's') {
        char *str = va_arg(args, char*);
        serial_print(str);
      }

      if(*format == 'd') {
        int num = va_arg(args, int);
        serial_print(int_to_string(num));
      }

      if(*format == 'f') {
        double num = va_arg(args, double);
        serial_print(double_to_string(num));
      }

      if(*format == 'l') {
        format++;
        if(*format == 'f') {
          double num = va_arg(args, double);
          serial_print(double_to_string_precise(num, 6));
        }

      }

      if(*format == 'x') {
        int num = va_arg(args, int);
        serial_print(hex_to_string(num));
      }

    } else {
      char str[2] = {*format, '\0'};
      serial_print(str);
    }

    format++;
  }

  return 0;
}
