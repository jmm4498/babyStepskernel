#ifndef H_SIMPLEFONT
#define H_SIMPLEFONT

typedef struct {

	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;

} PSF1_HEADER;

typedef struct {

	PSF1_HEADER *psf1_header;
	void *glyph_buffer;

} PSF1_FONT;

#endif