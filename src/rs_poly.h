#ifndef RS_POLY_H
#define RS_POLY_H

#include <stdint.h>
#include "rs_field.h"


#define MAX_POLY  1024       

int  poly_deg       (const uint32_t *p, int max_d);          
void poly_copy      (uint32_t *dst, const uint32_t *src, int len);
void poly_set_zero  (uint32_t *p,   int len);

void poly_scalar_mul(uint32_t *p,   int deg, uint32_t c);    // p*=c
void poly_scalar_div(uint32_t *p,   int deg, uint32_t c);    
void poly_addto     (uint32_t *a,   const uint32_t *b, int deg_b);  //a+=b 
void poly_subfrom   (uint32_t *a,   const uint32_t *b, int deg_b);  //a-=b

void poly_mul       (const uint32_t *a,int da,
                     const uint32_t *b,int db,
                     uint32_t *res); // res[]=a·b 

void poly_div       (const uint32_t *num,int dnum,
                     const uint32_t *den,int dden,
                     uint32_t *quo, // Q(x) 
                     uint32_t *rem);     //R(x)

uint32_t poly_eval  (const uint32_t *p,int deg,uint32_t x);

#endif /* RS_POLY_H */
