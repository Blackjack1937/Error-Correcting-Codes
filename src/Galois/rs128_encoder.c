 #include <stdio.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include <time.h>
 #include "gf128.h"

 #define GF_SIZE 128    

 

 void rs128_encode(const gf *msg,int k,int n,gf *code)
 {
     for (int i = 0; i < n; ++i) {
        //alpha_i = i+1
         gf a = gf_pow(0x02, i);      
         gf y = 0;
         for (int j = k - 1; j >= 0; --j)         
             y = gf_add(gf_mul(y, a), msg[j]);
         code[i] = y;                             
     }
 }

 static void die(const char *msg) { fputs(msg, stderr); exit(EXIT_FAILURE); }
 static gf urand32(void)   { return (gf)(rand() % GF_SIZE); }
 


 #ifdef RS_ENCODER   
 int main(int argc, char **argv)
 {
     if (argc < 3)
         die("Usage: rs_encoder k n [m0 m1 … mk-1]\n");
 
     int k = atoi(argv[1]);
     int n = atoi(argv[2]);
     if (k <= 0 || n < k || n > (int)PRIME)
         die("Invalid k or n. Must satisfy 1 ≤ k ≤ n ≤ 65537.\n");
 
     gf *msg   = malloc(k * sizeof(*msg));
     gf *code  = malloc(n * sizeof(*code));
     gf *check = malloc(n * sizeof(*check));
     if (!msg || !code || !check) die("Memory allocation error\n");
     if (argc == 3 + k) {
         for (int i = 0; i < k; ++i) {
             long v = strtol(argv[3 + i], NULL, 10);
             if (v < 0 || v >= PRIME) die("Coefficient out of range\n");
             msg[i] = (gf)v;
         }
     } else if (argc == 3) {
         srand((unsigned)time(NULL));
         for (int i = 0; i < k; ++i) msg[i] = urand32();
     } else
         die("Provide exactly k coefficients or none\n");
     rs_encode(msg, k, n, code);
     rs_encode(msg, k, n, check); //verify encoding
     int valid = 1;
     for (int i = 0; i < n && valid; ++i)
         if (check[i] != code[i]) valid = 0;
 
     // print res
     printf("Message coefficients (%d):\n", k);
     for (int i = 0; i < k; ++i)
         printf("%u%c", msg[i], i + 1 == k ? '\n' : ' ');


     printf("\nCode-word (%d):\n", n);
     for (int i = 0; i < n; ++i)
         printf("%u%c", code[i], i + 1 == n ? '\n' : ' ');
     printf("\n%s\n", valid ? "--- VALID ---" : "--- FAILED ! ---");
     
     //cleanup
     free(msg);
     free(code);
     free(check);
     return valid ? 0 : 1;
 }
 
 #endif