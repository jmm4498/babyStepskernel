#include "renderer.h"



void __renderer_init(Renderer *renderer, Framebuffer *framebuffer, PSF1_FONT *psf1_font, unsigned int color) {

    renderer->framebuffer = framebuffer;
    renderer->psf1_font = psf1_font;
    renderer->point.x = 0;
    renderer->point.y = 0;
    renderer->color = color;

    return;
}


void __renderer_set_cursor(Renderer *renderer, unsigned int x, unsigned int y) {

    if(renderer == NULL) {
        return;
    }

    renderer->point.x = x;
    renderer->point.y = y;

    return;
}

void __renderer_set_color(Renderer *renderer, unsigned int color) {

    if(renderer == NULL) {
        return;
    }

    renderer->color = color;

    return;
}