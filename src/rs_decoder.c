#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rs_field.h"
#include "rs_encoder.h"

#define MAX_N   1024
#define MAX_T   256

static uint32_t urand32(void){return (uint32_t)(rand()%PRIME);}
static uint32_t eval_poly(const uint32_t *p,int deg,uint32_t x){
    uint32_t y=0;for(int i=deg;i>=0;--i)y=mod_add(mod_mul(y,x),p[i]);return y;
}

static int berlekamp_massey(const uint32_t *s,int t,uint32_t *sigma){
    uint32_t C[MAX_T+1]={1},B[MAX_T+1]={1};
    int L=0,m=1;
    uint32_t b=1;
    for(int n=0;n<2*t;n++){
        uint32_t d=s[n];
        for(int i=1;i<=L;i++) d=mod_add(d,mod_mul(C[i],s[n-i]));
        if(d){
            uint32_t coef=mod_mul(d,mod_inv(b));
            uint32_t T[MAX_T+1]={0};
            for(int i=0;i<=L;i++) T[i+m]=mod_mul(coef,B[i]);
            for(int i=0;i<=t;i++) C[i]=mod_sub(C[i],T[i]);
            if(2*L<=n){
                L=n+1-L;b=d;memcpy(B,C,sizeof(uint32_t)*(t+1));m=1;
            }else m++;
        }else m++;
    }
    memcpy(sigma,C,(t+1)*sizeof(uint32_t));
    return L;
}

int rs_decode(int n,int k,const uint32_t *recv,uint32_t *corr){
    int t=(n-k)/2;
    uint32_t S[MAX_T*2]={0};int nonzero=0;
    for(int j=0;j<2*t;j++){
        uint32_t Sj=0;
        for(int i=0;i<n;i++) Sj=mod_add(Sj,mod_mul(recv[i],mod_pow(i+1,j)));
        S[j]=Sj;if(Sj) nonzero=1;
    }
    if(!nonzero){memcpy(corr,recv,n*sizeof(uint32_t));return 0;}
    uint32_t sigma[MAX_T+1]={0};
    int L=berlekamp_massey(S,t,sigma);
    if(L>t||sigma[0]==0) return -1;
    uint32_t der[MAX_T]={0};for(int i=1;i<=L;i++) der[i-1]=mod_mul(i,sigma[i]);
    memcpy(corr,recv,n*sizeof(uint32_t));int errs=0;
    for(int i=0;i<n;i++){
        if(eval_poly(sigma,L,i+1)==0){
            uint32_t xi_inv=mod_inv(i+1);
            uint32_t num=0;for(int j=0;j<L;j++) num=mod_add(num,mod_mul(S[j],mod_pow(xi_inv,j)));
            uint32_t den=eval_poly(der,L-1,i+1);
            corr[i]=mod_sub(corr[i],mod_mul(num,mod_inv(den)));
            errs++;
        }
    }
    return errs;
}

int main(int argc,char **argv){
    if(argc<4){printf("usage: rs_decoder n k t\n");return 0;}
    int n=atoi(argv[1]),k=atoi(argv[2]),t_req=atoi(argv[3]);
    uint32_t *msg=malloc(k*sizeof(uint32_t));
    uint32_t *code=calloc(n,sizeof(uint32_t));
    uint32_t *recv=calloc(n,sizeof(uint32_t));
    uint32_t *dec =calloc(n,sizeof(uint32_t));
    srand((unsigned)time(NULL));
    for(int i=0;i<k;i++) msg[i]=urand32();
    rs_encode(msg,k,n,code);memcpy(recv,code,n*sizeof(uint32_t));
    int inj=rand()%(t_req+1);
    for(int j=0;j<inj;j++) recv[rand()%n]=urand32();
    int fixed=rs_decode(n,k,recv,dec);
    int ok=1;for(int i=0;i<n;i++) if(dec[i]!=code[i]){ok=0;break;}
    printf("injected %d  corrected %d  %s\n",inj,fixed,ok?"OK":"FAIL");
    return ok?0:1;
}
