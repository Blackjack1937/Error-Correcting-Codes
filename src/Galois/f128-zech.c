// f128-zech.c
#include <stdio.h>
#include <stdint.h>
#include "f128.h" 

static uint8_t log_table[128]; 
static uint8_t exp_table[128]; // only need 127 but keep 128 for safety



void f128_zech_init() {
    //  Start with alpha^0 = 1
    uint8_t val = 1;
    for (int i = 0; i < 127; i++) {
        exp_table[i] = val;
        if (i > 0) {
            log_table[val] = i;
        }
        val = f128_mul(val, 0x02); //multiply by alpha in GF(2^7)
    }
    log_table[1] = 0;  //log of 1 is 0
}



uint8_t f128_zech_mul(uint8_t x, uint8_t y) {
    if (x == 0 || y == 0) return 0;
    uint8_t lx = log_table[x];
    uint8_t ly = log_table[y];
    uint8_t sum = (lx + ly) % 127; 
    return exp_table[sum];
}

uint8_t f128_zech_inv(uint8_t x) {
    if (x == 0) return 0; // no inverse
    uint8_t lx = log_table[x];
    uint8_t inv_exp = (127 - lx) % 127; 
    return exp_table[inv_exp];
}

uint8_t f128_zech_add(uint8_t x, uint8_t y) {
    return x ^ y; // same as polynomial addition
}
