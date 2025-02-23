#include "cstr.h"

char int_buffer_out[MAX_BUF_OUT_INT];
char uint_buffer_out[MAX_BUF_OUT_INT];
char double_buffer_out[MAX_BUF_OUT_DOUBLE];

const char* uint_to_string(uint64_t value) {

    uint8_t size;
    uint8_t index = 0;
    uint64_t size_test = value;
    const char *ptr;

    while(size_test / 10 > 0) {
        size_test /= 10;
        size++;
    }

    while(value / 10 > 0) {

        uint8_t remainder = value % 10;
        value /= 10;

        uint_buffer_out[size - index] = remainder + '0';
        index++;
    }

    uint8_t remainder = value % 10;
    uint_buffer_out[size - index] = remainder + '0';
    uint_buffer_out[size + 1] = 0;

    ptr = uint_buffer_out;

    return ptr;
}

const char* int_to_string(int64_t value) {

    const char *ptr;
    uint8_t size = 0;
    uint8_t index = 0;
    uint8_t is_negative = 0;
    uint64_t size_test = 0;

    if(value < 0) {
        is_negative = 1;
        value *= -1; // Flip to positive
        int_buffer_out[0] = '-';
    }

    
    size_test = (uint64_t)value;
    while(size_test / 10 > 0) {
        size_test /= 10;
        size++;
    }

    while(value / 10 > 0) {

        uint8_t remainder = value % 10;
        value /= 10;

        int_buffer_out[is_negative + size - index] = remainder + '0';
        index++;
    }

    uint8_t remainder = value % 10;
    int_buffer_out[is_negative + size - index] = remainder + '0';
    int_buffer_out[is_negative + size + 1] = 0;

    ptr = int_buffer_out;

    return ptr;
}

const char *double_to_string(double value, uint8_t decimal_places) {

    return (void *)0;
}