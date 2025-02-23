#include "direct_io.h"
/** Low level print functions */

/** Prints a character directly to the screen with a renderer's given font  */
void putchar_DIRECT(Renderer *renderer, unsigned int color, char chr, unsigned int xoffset, unsigned int yoffset) {

    unsigned int *pixel = (unsigned int*)renderer->framebuffer->base_address;
    unsigned char *fontptr = renderer->psf1_font->glyph_buffer + (chr * renderer->psf1_font->psf1_header->charsize);

    for(unsigned long y = yoffset; y < yoffset + 16; y++) {
        for(unsigned x = xoffset; x < xoffset + 8; x++) {
            if ((*fontptr & (0b10000000 >> (x - xoffset))) > 0) {
                *(unsigned int*)(pixel + x + (y * renderer->framebuffer->pixels_per_sl)) = color;
            }
        }
        fontptr++;
    }

    return;
}

/** Prints a given string starting at a given renderer's cursor position */
void print_DIRECT(Renderer *renderer, const char *str) {

    unsigned int x_step_size = renderer->psf1_font->psf1_header->charsize / 2;
    unsigned int y_step_size = renderer->psf1_font->psf1_header->charsize;

    for(unsigned int i = 0; str[i] != 0; i++) {

        if(str[i] == '\n') {
            __renderer_set_cursor(renderer, 0, renderer->point.y + y_step_size);
            continue;
        }

        putchar_DIRECT(renderer, renderer->color, str[i], renderer->point.x, renderer->point.y);
        __renderer_set_cursor(renderer, renderer->point.x + x_step_size, renderer->point.y);
    }

    return;
}