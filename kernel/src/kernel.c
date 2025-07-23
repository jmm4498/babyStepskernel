#include "defs.h"
#include "renderer.h"
#include "direct_io.h"
#include "cstr.h"

void _start(Framebuffer *framebuffer, PSF1_FONT *psf1_font) {

    Renderer renderer;

    __renderer_init(&renderer, framebuffer, psf1_font, OS_COLOR_STD);

    __renderer_set_cursor(&renderer, 15, 0);
    
    for(int i = 0; i < 20; i++) { 
        print_DIRECT(&renderer, hex_to_string((uint64_t)0x1234));
        __renderer_set_cursor(&renderer, 15, (i + 1) * 16);
    }

  

    //print_DIRECT(&renderer, uint_to_string((uint64_t)123456));
    //print_DIRECT(&renderer, int_to_string((int64_t)-123456));

    

    return;
}
