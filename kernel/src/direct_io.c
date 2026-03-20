#include "direct_io.h"
#include "kernel_lib.h"
#include <stdint.h>
/** Low level print functions */

/** Prints a character directly to the screen with a renderer's given font  */
void putchar_DIRECT(Renderer *renderer, unsigned int color, char chr,
                    unsigned int xoffset, unsigned int yoffset) {

  unsigned int *pixel = (unsigned int *)renderer->framebuffer->back_buffer;
  unsigned char *fontptr = renderer->psf1_font->glyph_buffer +
                           (chr * renderer->psf1_font->psf1_header->charsize);

  for (unsigned long y = yoffset; y < yoffset + 16; y++) {
    for (unsigned x = xoffset; x < xoffset + 8; x++) {
      if ((*fontptr & (0b10000000 >> (x - xoffset))) > 0) {
        *(unsigned int *)(pixel + x +
                          (y * renderer->framebuffer->pixels_per_sl)) = color;
      }
    }
    fontptr++;
  }

  __memcpy(renderer->framebuffer->back_buffer,
           renderer->framebuffer->base_address,
           renderer->framebuffer->buffer_size);

  return;
}

/** Prints a given string starting at a given renderer's cursor position */
void print_DIRECT(Renderer *renderer, const char *str) {

  unsigned int x_step_size = renderer->psf1_font->psf1_header->charsize / 2;
  unsigned int y_step_size = renderer->psf1_font->psf1_header->charsize;

  for (unsigned int i = 0; str[i] != 0; i++) {

    if (str[i] == '\n') {
      __renderer_set_cursor(renderer, 0, renderer->point.y + y_step_size);
      continue;
    }

    putchar_DIRECT(renderer, renderer->color, str[i], renderer->point.x,
                   renderer->point.y);
    __renderer_set_cursor(renderer, renderer->point.x + x_step_size,
                          renderer->point.y);
  }

  return;
}

/** Print functions for writing directly to a serial port **/
static inline uint8_t inb(uint16_t port) {
  uint8_t ret;

  asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));

  return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
  asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static void serial_wait_ready(void) {
  while ((inb(0x3F8 + 5) & 0x20) == 0) {
    // spin
  }

  return;
}

void serial_putchar(char c) {
  if (c == '\n') {
    serial_putchar('\r');
  }

  serial_wait_ready();

  outb(0x3F8, (uint8_t)c);

  return;
}

void serial_print(const char *s) {
  while (*s) {
    serial_putchar(*s++);
  }
}

void serial_print_hex(uint64_t value) {

  char buf[19];
  buf[0] = '0';
  buf[1] = 'x';
  int i = 17;
  buf[i--] = '\0';

  do {
    uint8_t digit = value & 0xF;
    buf[i--] = digit < 10 ? '0' + digit : 'A' + digit - 10;
    value >>= 4;
  } while (value && i > 1);

  while (i > 1)
    buf[i--] = '0';

  serial_print(buf);

  return;
}

void serial_init(void) {

  outb(0x3F8 + 1, 0x00);
  outb(0x3F8 + 3, 0x80);
  outb(0x3F8 + 0, 0x03);
  outb(0x3F8 + 1, 0x00);
  outb(0x3F8 + 3, 0x03);
  outb(0x3F8 + 2, 0xC7);
  outb(0x3F8 + 4, 0x0B);

  return;
}
