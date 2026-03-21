#ifndef H_IO_H
#define H_IO_H

#include "direct_io.h"
#include <stdarg.h>

int kprintf(const char *format, ...);

int serial_printf(const char *format, ...);

#endif
