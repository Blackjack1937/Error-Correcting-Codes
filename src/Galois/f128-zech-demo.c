#include <stdio.h>
#include "f128.h" 
uint8_t f128_zech_mul(uint8_t x, uint8_t y);
uint8_t f128_zech_inv(uint8_t x);
uint8_t f128_zech_add(uint8_t x, uint8_t y);
void    f128_zech_init(void);

static void print_elem(const char *label, uint8_t x) {
    printf("%s = 0x%02X\n", label, x);
}


int main() {
    printf("===== F128 Zech-Log Demo =====\n");
    f128_zech_init(); 

    uint8_t x = 0x53;
    uint8_t y = 0x75;

    print_elem("x", x);
    print_elem("y", y);

    uint8_t s = f128_zech_add(x, y);
    print_elem("x + y", s);

    uint8_t m = f128_zech_mul(x, y);
    print_elem("x * y", m);

    uint8_t ix = f128_zech_inv(x);
    print_elem("x^{-1}", ix);

    uint8_t check = f128_zech_mul(x, ix);
    print_elem("x*x^{-1}", check);

    return 0;
}
