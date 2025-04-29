 #include <stdio.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include <time.h>
 #include "rs_field.h"

 

 void rs_encode(const uint32_t *msg, int k, int n, uint32_t *code)
 {
     for (int i = 0; i < n; ++i) {
        //alpha_i = i+1
         uint32_t a = (uint32_t)(i + 1);      
         uint32_t y = 0;
         for (int j = k - 1; j >= 0; --j)         
             y = mod_add(mod_mul(y, a), msg[j]);
         code[i] = y;                             
     }
 }

 static void die(const char *msg) { fputs(msg, stderr); exit(EXIT_FAILURE); }
 static uint32_t urand32(void)   { return (uint32_t)(rand() % PRIME); }
 


 #ifdef RS_ENCODER   
 int main(int argc, char **argv)
 {
     if (argc < 3)
         die("Usage: rs_encoder k n [m0 m1 … mk-1]\n");
 
     int k = atoi(argv[1]);
     int n = atoi(argv[2]);
     if (k <= 0 || n < k || n > (int)PRIME)
         die("Invalid k or n. Must satisfy 1 ≤ k ≤ n ≤ 65537.\n");
 
     uint32_t *msg   = malloc(k * sizeof(*msg));
     uint32_t *code  = malloc(n * sizeof(*code));
     uint32_t *check = malloc(n * sizeof(*check));
     if (!msg || !code || !check) die("Memory allocation error\n");
     if (argc == 3 + k) {
         for (int i = 0; i < k; ++i) {
             long v = strtol(argv[3 + i], NULL, 10);
             if (v < 0 || v >= PRIME) die("Coefficient out of range\n");
             msg[i] = (uint32_t)v;
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