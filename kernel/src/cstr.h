#ifndef H_CSTR
#define H_CSTR
#include <stdint.h>

#define MAX_BUF_OUT_INT 128
#define MAX_BUF_OUT_DOUBLE 128

const char *uint_to_string(uint64_t value);
const char *int_to_string(int64_t value);
const char *double_to_string(double value, uint8_t decimal_places);


#endif