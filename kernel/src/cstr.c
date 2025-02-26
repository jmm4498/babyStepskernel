#include "cstr.h"

char int_buffer_out[MAX_BUF_OUT_INT];
char uint_buffer_out[MAX_BUF_OUT_INT];
char double_buffer_out[MAX_BUF_OUT_DOUBLE];
char hex_buffer_out[MAX_BUF_OUT_INT];

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

const char *double_to_string_precise(double value, uint8_t decimal_places) {

    if(decimal_places > 20) {
        decimal_places = 20;
    }

    char *intptr = (char*)int_to_string((int64_t) value);
    char *doubleptr = double_buffer_out;

    if(value < 0) {
        value *= -1;
    }

    while(*intptr != 0) {
        *doubleptr = *intptr;
        intptr++;
        doubleptr++;
    }

    *doubleptr = '.';
    doubleptr++;

    double new_value = value - (int) value; //get decimal value 

    for(uint8_t i = 0; i < decimal_places; i++) {
        new_value *= 10;
        *doubleptr = (int) new_value + '0';
        new_value -= (int) new_value;
        doubleptr++;
    }

    *doubleptr = 0;
    return double_buffer_out;
}

const char *double_to_string(double value) {
    return double_to_string_precise(value, 2);
}

const char *hex_to_string(uint64_t value) {

    uint64_t *valptr = &value;
    uint8_t *ptr;
    uint8_t temp;
    uint8_t size = 8 * 2 - 1;

    for(uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t*)valptr + i);
        temp = ((*ptr & 0xF0) >> 4);
        hex_buffer_out[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : '0');
        temp = ((*ptr & 0x0F));
        hex_buffer_out[size - (i * 2)] = temp + (temp > 9 ? 55 : '0');
    }

    hex_buffer_out[size + 1] = 0;
    return hex_buffer_out;
}