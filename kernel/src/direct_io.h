/** Low level print functions */
#ifndef H_DIR_IO
#define H_DIR_IO
#include "renderer.h"

/** Prints a character directly to the screen with a renderer's given font  */
void putchar_DIRECT(Renderer *renderer, unsigned int color, char chr, unsigned int xoffset, unsigned int yoffset);


/** Prints a given string starting at a given renderer's cursor position */
void print_DIRECT(Renderer *renderer, const char *str);
#endif