#ifndef H_FRAMEBUFFER
#define H_FRAMEBUFFER

#include <stddef.h>

typedef struct {

	void *base_address;
  void *back_buffer;
	size_t buffer_size;
	unsigned int width;
	unsigned int height;
	unsigned int pixels_per_sl;

} Framebuffer;

#endif
