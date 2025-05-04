#include <string.h>
#include "../rs_poly.h"  
#include "gf128.h"

int poly_deg(const uint32_t *p,int max_d){
    for(int i=max_d;i>=0;--i) if(p[i]) return i;
    return -1;
}
void poly_copy(uint32_t *d,const uint32_t*s,int len){ memcpy(d,s,len*sizeof(*d)); }
void poly_set_zero(uint32_t *p,int len){ memset(p,0,len*sizeof(*p)); }

void poly_scalar_mul(uint32_t *p,int deg,uint32_t c){
    for(int i=0;i<=deg;++i) p[i]=mod_mul(p[i],c);
}
void poly_scalar_div(uint32_t *p,int deg,uint32_t c){
    uint32_t inv=mod_inv(c);
    for(int i=0;i<=deg;++i) p[i]=mod_mul(p[i],inv);
}
void poly_addto(uint32_t *a,const uint32_t *b,int db){
    for(int i=0;i<=db;++i) a[i]=mod_add(a[i],b[i]);
}
void poly_subfrom(uint32_t *a,const uint32_t *b,int db){
    for(int i=0;i<=db;++i) a[i]=mod_sub(a[i],b[i]);
}

// multiplication 
void poly_mul(const uint32_t *a,int da,const uint32_t *b,int db,uint32_t *r){
    poly_set_zero(r,da+db+1);
    for(int i=0;i<=da;++i)
        for(int j=0;j<=db;++j)
            r[i+j]=mod_add(r[i+j],mod_mul(a[i],b[j]));
}

//long division  
void poly_div(const uint32_t *num,int dnum,
              const uint32_t *den,int dden,
              uint32_t *quo,uint32_t *rem)
{
    poly_set_zero(quo,dnum);
    poly_copy(rem,num,dnum+1);

    int deg_r = dnum;
    uint32_t den_lead_inv = mod_inv(den[dden]);

    while(deg_r >= dden && deg_r>=0){
        uint32_t coef = mod_mul(rem[deg_r], den_lead_inv);
        int shift = deg_r - dden;
        quo[shift] = coef;
        for(int i=0;i<=dden;++i){
            rem[i+shift]=mod_sub(rem[i+shift],
                                 mod_mul(coef,den[i]));
        }
        while(deg_r>=0 && rem[deg_r]==0) --deg_r;
    }
}

uint32_t poly_eval(const uint32_t *p,int deg,uint32_t x){
    uint32_t y=0;
    for(int i=deg;i>=0;--i)
        y=mod_add(mod_mul(y,x),p[i]);
    return y;
}
