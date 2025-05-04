#ifndef F128_H
#define F128_H

#include <stdint.h>

// sum
uint8_t f128_add(uint8_t a, uint8_t b);
uint8_t f128_sub(uint8_t a, uint8_t b);
uint8_t f128_neg(uint8_t a);

// mul
uint8_t f128_mul(uint8_t a, uint8_t b);
uint8_t f128_inv(uint8_t a);
uint8_t f128_div(uint8_t a, uint8_t b);

#endif
