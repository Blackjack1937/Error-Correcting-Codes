#include <stdio.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include <time.h>
 #include "rs_field.h"
 
 void rs_encode(const uint32_t *msg, int k, int n, uint32_t *code);
 
 static void die(const char *msg)          { fputs(msg, stderr); exit(EXIT_FAILURE); }
 static uint32_t urand32(void)             { return (uint32_t)(rand() % PRIME); }
 static uint32_t mod_pow32(uint32_t a,int e){ uint32_t r=1; while(e--) r=mod_mul(r,a); return r; }
 
 static void test_golden(void)
 {
     const int k = 5, n = 8;
     uint32_t msg[5]  = {1,2,3,4,5};
     uint32_t want[8] = {15,129,547,1593,3711,7465,13539,22737};
     uint32_t got[8];
 
     rs_encode(msg,k,n,got);
 
     for (int i=0;i<n;++i)
         if (got[i]!=want[i])
             die("golden-vector mismatch\n");
     puts("test_golden … OK");
 }

 static void test_identity(int k,int n,int trials,unsigned seed)
 {
     srand(seed);
     uint32_t *m = malloc(k*sizeof(*m));
     uint32_t *c = malloc(n*sizeof(*c));
     if(!m||!c) die("malloc failed");
 
     for (int t=0;t<trials;++t){
         for(int i=0;i<k;++i) m[i]=urand32();
         rs_encode(m,k,n,c);
 
         for(int i=0;i<n;++i){
             uint32_t rhs=0, a=i+1;
             for(int j=0;j<k;++j)
                 rhs = mod_add(rhs, mod_mul(m[j],mod_pow32(a,j)));
             if(rhs!=c[i]) die("identity property failed\n");
         }
     }
     puts("test_identity … OK");
     free(m); free(c);
 }
 
 int main(int argc,char**argv)
 {
     int k=20;
     int n=31;
     int trials=500; 
     unsigned seed=1234;
     if(argc==5){
         k     = atoi(argv[1]);
         n     = atoi(argv[2]);
         trials= atoi(argv[3]);
         seed  = (unsigned)atoi(argv[4]);
     }else if(argc!=1){
         fprintf(stderr,
             "Usage: %s [k n trials seed]\n"
             "Default: k=20 n=31 trials=500 seed=1234\n", argv[0]);
         return EXIT_FAILURE;
     }
     if(k<=0||n<k||n>(int)PRIME) die("Invalid k,n\n");
 
     test_golden();
     test_identity(k,n,trials,seed);
     puts("All encoder unit tests passed ✓");
     return 0;
 }
 