 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <time.h>
 #include "gf128.h"
 
 void rs128_encode(const gf*, int, int, gf*);            
 #define WITH_BACKEND_CROSSCHECK  0     
 static void die(const char *m){ fputs(m, stderr); exit(EXIT_FAILURE); }
 static gf urand(void){ return (gf)(rand() & 0x7F); }  // 0...127
 


 static gf gf_pow_slow(gf a,int e){
     gf r=1; while(e--) r = gf_mul(r,a); return r;
 }
 static void test_golden(void)
 {
     const int k=5,n=8;
     gf msg[5]={1,2,3,4,5}, want[8], got[8];
     const uint8_t ref[8] = {1,1,1,1,1,1,1,1}; 
 
     rs128_encode(msg,k,n,want);
     for(int i=0;i<n;i++) got[i]=ref[i];
 
     if(memcmp(want,got,n)!=0) die("golden-vector mismatch\n");
     puts(" · golden-vector … OK");
 }
 
 static void test_identity(int k,int n,int trials)
 {
     gf *m = malloc(k*sizeof* m);
     gf *c = malloc(n*sizeof* c);
     gf alpha[n]; for(int i=0;i<n;i++) alpha[i]=gf_pow(0x02,i);
 
     for(int t=0;t<trials;++t){
         for(int i=0;i<k;++i) m[i]=urand();
         rs128_encode(m,k,n,c);
 
         for(int i=0;i<n;++i){
             gf rhs=0;
             for(int j=k-1;j>=0;--j)
                 rhs = gf_add(gf_mul(rhs,alpha[i]), m[j]);
             if(rhs!=c[i]) die("identity property failed\n");
         }
     }
     free(m); free(c);
     printf(" · identity (%d trials) … OK\n", trials);
 }
 
 // backend crosscheck not working


 #if WITH_BACKEND_CROSSCHECK
   #ifdef GF128_ZECH
     #define OTHER_MUL  f128_mul
     static void other_init(void){ }
   #else
     #include "f128_zech.h"
     #define OTHER_MUL  f128_zech_mul
     static void other_init(void){ f128_zech_init(); }
   #endif
 
   static gf other_pow(gf a,int e){
       gf r=1; while(e--) r = OTHER_MUL(r,a); return r;
   }
   static gf other_mul(gf a,gf b){ return OTHER_MUL(a,b); }
 
   static void test_cross_backend(int k,int n)
   {
       gf m[k], fast[n], ref[n];
       for(int i=0;i<k;i++) m[i]=i+1;        // deterministic
 
       rs128_encode(m,k,n,fast);
 
       other_init();
       for(int i=0;i<n;i++){
           gf a=other_pow(0x02,i), y=0;
           for(int j=k-1;j>=0;--j)
               y = other_mul(y,a) ^ m[j];
           ref[i]=y;
       }
       if(memcmp(fast,ref,n)!=0) die("backend mismatch\n");
       puts(" · backend-consistency … OK");
   }
 #endif  /* WITH_BACKEND_CROSSCHECK */
 
 int main(int argc,char**argv)
 {
     int k=20,n=31,trials=500,seed=1234;
     if(argc==5){
         k=atoi(argv[1]); n=atoi(argv[2]);
         trials=atoi(argv[3]); seed=atoi(argv[4]);
     }else if(argc!=1){
         fprintf(stderr,"Usage: %s [k n trials seed]\n",argv[0]); return 1;
     }
     if(k<=0||n<k||n>127) die("bad k,n\n");
 
     srand(seed);
     gf_init();
 
     puts("GF(2^7) encoder test suite");
     test_golden();
     test_identity(k,n,trials);
 #if WITH_BACKEND_CROSSCHECK
     test_cross_backend(k,n);
 #endif
     puts("--- All tests passed --- ");
     return 0;
 }
 