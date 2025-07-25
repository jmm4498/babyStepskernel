#ifndef H_RENDERER
#define H_RENDERER

#include <stddef.h>
#include "framebuffer.h"
#include "simplefonts.h"
#include "math.h"

typedef struct {
    unsigned int color;
    Framebuffer *framebuffer; //points to the actual framebuffer in memory
    PSF1_FONT *psf1_font; //might get rid of this reference
    Point point;
} Renderer; 

void __renderer_init(Renderer *renderer, Framebuffer *framebuffer, PSF1_FONT *psf1_font, unsigned int color);

void __renderer_set_cursor(Renderer *renderer, unsigned int x, unsigned int y);

void __renderer_set_color(Renderer *renderer, unsigned int color);

#endif
