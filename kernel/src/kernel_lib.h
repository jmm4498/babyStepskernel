#ifndef H_KERNELLIB_H
#define H_KERNELLIB_H

#include <stddef.h>
#include <stdint.h>

int __memcpy(void *src, void *dest, size_t n);

void *__memset(void *dest, int c, size_t n);

#endif
