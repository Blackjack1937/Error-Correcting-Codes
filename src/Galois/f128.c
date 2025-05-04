// f128.c
#include <stdint.h>
#include "f128.h"
#define IRRED 0x83 


uint8_t f128_add(uint8_t a, uint8_t b) {
    return a ^ b;
}
uint8_t f128_sub(uint8_t a, uint8_t b) {
    return a ^ b;  
}
uint8_t f128_neg(uint8_t a) {
    return a;     
}

uint8_t f128_mul(uint8_t a, uint8_t b) {
    uint8_t result = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) {
            result ^= a; 
        }
        b >>= 1;

        uint8_t high = a & 0x80; 
        a <<= 1;
        if (high) {
            a ^= (IRRED & 0xFF); 
        }
    }
    return result;
}

static uint8_t f128_inv_extended_euclid(uint8_t a) {
    if (a == 0) {
        return 0; 
    }

    uint8_t r0 = a, r1 = 0x83; 
    uint8_t s0 = 1, s1 = 0;

    while (r1 != 0 && r1 != 1) {
        // compute degree
        int deg_r0 = 7 - __builtin_clz(r0); // find highest set bit
        int deg_r1 = 7 - __builtin_clz(r1);
        if (deg_r0 < deg_r1) {
            // swap
            uint8_t tmp = r0; r0 = r1; r1 = tmp;
            tmp = s0; s0 = s1; s1 = tmp;
            int tempd = deg_r0; deg_r0 = deg_r1; deg_r1 = tempd;
        }
        int shift = deg_r0 - deg_r1;
        uint8_t temp = r1 << shift;
        r0 ^= temp;


        uint8_t t2 = s1;
        for (int i = 0; i < shift; i++) {
            // shift left
            uint8_t h = (t2 & 0x80);
            t2 <<= 1;
            if (h) {
                t2 ^= 0x83;
            }
        }
        s0 ^= t2;
    }

    if (r1 == 0) {
        return 0; 
    }
    return (r1 == 1) ? s1 : s0; 
}

uint8_t f128_inv(uint8_t a) {
    return f128_inv_extended_euclid(a);
}

uint8_t f128_div(uint8_t a, uint8_t b) {
    uint8_t invb = f128_inv(b);
    return f128_mul(a, invb);
}
