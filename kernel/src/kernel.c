#include "defs.h"
#include "renderer.h"
#include "direct_io.h"
#include "cstr.h"
#include "kernel_lib.h"

void _start(Framebuffer *framebuffer, PSF1_FONT *psf1_font) {

    Renderer renderer;

    __renderer_init(&renderer, framebuffer, psf1_font, OS_COLOR_STD);

    __renderer_set_cursor(&renderer, 45, 0);

    const char *test = "Hello World!\n";
    char buf[64] = { '\0' };

    if(__memcpy((void*)test, buf, 13) != SUCCESS) {
      print_DIRECT(&renderer, "__memcpy() failed in kernel!\n");
    }

    buf[13] = '\0';

    print_DIRECT(&renderer, test);
    print_DIRECT(&renderer, buf);

    //for(int i = 0; i < 20; i++) { 
    //    print_DIRECT(&renderer, hex_to_string((uint64_t)0x1234));
    //    __renderer_set_cursor(&renderer, 15, (i + 1) * 16);
    //}

  

    //print_DIRECT(&renderer, uint_to_string((uint64_t)123456));
    //print_DIRECT(&renderer, int_to_string((int64_t)-123456));

    

    return;
}
