#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "rs_field.h"
#include "rs_poly.h"
#include "rs_encoder.h"

#define MAX_N  1024
#define MAX_T  256

static void interpolate(const uint32_t *x,const uint32_t *y,int n,
                        uint32_t *dst)
{
    poly_set_zero(dst,n);
    uint32_t basis[MAX_N];

    for(int i=0;i<n;++i){
        poly_set_zero(basis,n); basis[0]=1; int d_b=0;
        for(int j=0;j<n;++j) if(j!=i){
            uint32_t fac[2]={mod_sub(0,x[j]),1},tmp[MAX_N]={0};
            poly_mul(basis,d_b, fac,1, tmp);
            d_b++; memcpy(basis,tmp,(d_b+1)*sizeof(uint32_t));
        }
        uint32_t denom=1;
        for(int j=0;j<n;++j) if(j!=i)
            denom = mod_mul(denom, mod_sub(x[i],x[j]));
        uint32_t scale = mod_mul(y[i], mod_inv(denom));
        poly_scalar_mul(basis,d_b,scale);
        poly_addto(dst,basis,d_b);
    }
}


int rs_decode(int n,int k,const uint32_t *recv,uint32_t *corr)
{
    if(n>MAX_N||(n-k)/2>MAX_T) return -1;
    int t=(n-k)/2;
    uint32_t alpha[MAX_N]; for(int i=0;i<n;++i) alpha[i]=i+1;
    uint32_t R[MAX_N];
    interpolate(alpha,recv,n,R);
    int d_R=poly_deg(R,n-1);
    uint32_t V[MAX_N+1]={0}; V[0]=1; int d_V=0;
    for(int i=0;i<n;++i){
        uint32_t fac[2]={mod_sub(0,alpha[i]),1},tmp[MAX_N+1]={0};
        poly_mul(V,d_V,fac,1,tmp);
        d_V++; memcpy(V,tmp,(d_V+1)*sizeof(uint32_t));
    }
    uint32_t r0[MAX_N+1],r1[MAX_N+1],v0[MAX_N+1],v1[MAX_N+1];
    memcpy(r0,V,(d_V+1)*sizeof(uint32_t)); int d0=d_V;
    memcpy(r1,R,(d_R+1)*sizeof(uint32_t)); int d1=d_R;
    poly_set_zero(v0,n+1);                 //v0=0 
    poly_set_zero(v1,n+1); v1[0]=1;        // v1=1 

    uint32_t q[MAX_N+1],r2[MAX_N+1],v2[MAX_N+1],tmp[MAX_N+1];

    while(d1>=k+t){
        poly_div(r0,d0,r1,d1,q,r2);
        int dq=poly_deg(q,d0-d1), dv1=poly_deg(v1,n);
        poly_mul(q,dq,v1,dv1,tmp);
        memcpy(v2,v0,(n+1)*sizeof(uint32_t));
        poly_subfrom(v2,tmp,poly_deg(tmp,n));
        memcpy(r0,r1,(d1+1)*sizeof(uint32_t)); d0=d1;
        memcpy(r1,r2,(d0+1)*sizeof(uint32_t)); d1=poly_deg(r1,d0);
        memcpy(v0,v1,(n+1)*sizeof(uint32_t));
        memcpy(v1,v2,(n+1)*sizeof(uint32_t));
    }

    int dv=poly_deg(v1,n);
    uint32_t lead_inv=mod_inv(v1[dv]);
    poly_scalar_mul(v1,dv,lead_inv);
    poly_scalar_mul(r1,d1,lead_inv);

    uint32_t m[MAX_N],rem[MAX_N];
    poly_div(r1,d1,v1,dv,m,rem);
    if(poly_deg(rem,k-1)>=0) return -1;   
    rs_encode(m,k,n,corr);

    int errs=0; for(int i=0;i<n;++i) if(corr[i]!=recv[i]) ++errs;
    return errs;
}

#ifdef RS_DECODER_MAIN
static uint32_t urand32(void){return (uint32_t)(rand()%PRIME);}
int main(void)
{
    const int k=20,n=31,t=(n-k)/2,trials=200;
    srand(2025);
    uint32_t *msg=malloc(k*sizeof*msg);
    uint32_t *enc=malloc(n*sizeof*enc);
    uint32_t *rec=malloc(n*sizeof*rec);
    uint32_t *dec=malloc(n*sizeof*dec);

    for(int tr=0;tr<trials;++tr){
        for(int i=0;i<k;++i) msg[i]=urand32();
        rs_encode(msg,k,n,enc);
        memcpy(rec,enc,n*sizeof(uint32_t));

        int inj=rand()%(t+1);
        bool used[MAX_N]={0};
        for(int e=0;e<inj;++e){
            int pos; do{pos=rand()%n;}while(used[pos]);
            used[pos]=true;
            uint32_t neo;
            do{neo=urand32();}while(neo==rec[pos]);
            rec[pos]=neo;
        }
        int fixed=rs_decode(n,k,rec,dec);
        if(fixed!=inj||memcmp(dec,enc,n*sizeof(uint32_t))){
            puts("FAIL"); return 1;
        }
    }
    puts("all OK");
    return 0;
}
#endif
