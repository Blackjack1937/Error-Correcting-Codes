#include <stdio.h>
#include "f128.h"

static void print_elem(const char *label, uint8_t x) {
    printf("%s = 0x%02X\n", label, x);
}

int main() {
    uint8_t x = 0x53; 
    uint8_t y = 0x75;  

    // core opps demo
    printf("===== F128 Demo (Polynomial Implementation) =====\n");

    print_elem("x", x);
    print_elem("y", y);

    uint8_t s  = f128_add(x, y);
    uint8_t d  = f128_sub(x, y);
    uint8_t nx = f128_neg(x);
    uint8_t m  = f128_mul(x, y);

    print_elem("x + y", s);
    print_elem("x - y", d);
    print_elem("-x   ", nx);
    print_elem("x * y", m);

    uint8_t ix = f128_inv(x);
    print_elem("x^{-1}", ix);
    // check: x * x^{-1} => 1?
    uint8_t check = f128_mul(x, ix);
    print_elem("x*x^{-1}", check);
    uint8_t divxy = f128_div(x, y);  //divison
    print_elem("x / y = x * y^{-1}", divxy);
    return 0;
}
