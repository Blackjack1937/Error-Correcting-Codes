#ifndef RS_FIELD_H
#define RS_FIELD_H

#include <stdint.h>

#define PRIME 65537u             

static inline uint32_t mod_add(uint32_t a, uint32_t b)
{ uint32_t s = a + b; return (s >= PRIME) ? s - PRIME : s; }

static inline uint32_t mod_sub(uint32_t a, uint32_t b)
{ return (a >= b) ? a - b : a + PRIME - b; }

static inline uint32_t mod_mul(uint32_t a, uint32_t b)
{ return (uint32_t)((uint64_t)a * b % PRIME); }

static inline uint32_t mod_pow(uint32_t a, uint32_t e)
{ uint64_t r = 1, base = a;
  while (e) { if (e & 1) r = r * base % PRIME; base = base * base % PRIME; e >>= 1; }
  return (uint32_t)r;
}

static inline uint32_t mod_inv(uint32_t a) 
{ return mod_pow(a, PRIME - 2); }


#endif /* RS_FIELD_H */
