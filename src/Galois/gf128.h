#ifndef GF128_H
#define GF128_H
#include <stdint.h>
#include "f128.h"
#include "f128-zech.h"

#ifdef GF128_ZECH
  #define gf_mul  f128_zech_mul
  #define gf_inv  f128_zech_inv
  static inline void gf_init(void){ f128_zech_init(); }
#else
  #define gf_mul  f128_mul
  #define gf_inv  f128_inv
  static inline void gf_init(void){ }          
#endif

#define gf_add(a,b)  ((a) ^ (b))                //XOR
#define gf_sub(a,b)  ((a) ^ (b))
typedef uint8_t      gf;                        

static inline gf gf_pow(gf a, unsigned e){
    gf r = 1;
    while(e){ if(e&1) r = gf_mul(r,a); a = gf_mul(a,a); e >>= 1; }
    return r;
}
#endif
