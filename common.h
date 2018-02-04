#ifndef COMMON_H

#define COMMON_H

#define NULL ((void *)(0))

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;

void delay(uint32_t nr_of_nops);

int set_string_zero(char *string, int size);
void get_hex_string(int num , char *hex_string, int string_size);

//void delay_us(uint32_t usecs);

#endif
