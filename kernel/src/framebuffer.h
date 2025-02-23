#ifndef H_FRAMEBUFFER
#define H_FRAMEBUFFER

#include <stddef.h>

typedef struct {

	void *base_address;
	size_t buffer_size;
	unsigned int width;
	unsigned int height;
	unsigned int pixels_per_sl;

} Framebuffer;

#endif