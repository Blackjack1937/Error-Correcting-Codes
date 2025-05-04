#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "rs_field.h"

void rs_encode (const uint32_t *msg,int k,int n,uint32_t *code);
int  rs_decode (int n,int k,const uint32_t *recv,uint32_t *corr);

#define MAX_N  1024         

static uint32_t urand32(void) { return (uint32_t)(rand() % PRIME); }
static void die(const char *m){ fputs(m,stderr); exit(1); }

static void inject_errors(uint32_t *v,int n,int inj)
{
    bool used[MAX_N] = {0};
    for (int e = 0; e < inj; ++e) {
        int pos;
        do { pos = rand() % n; } while (used[pos]);
        used[pos] = true;
        uint32_t old = v[pos], neo;
        do { neo = urand32(); } while (neo == old);
        v[pos] = neo;
    }
}

int main(int argc,char**argv)
{
    if (argc != 4) {
        fprintf(stderr,"usage: %s k n trials\n",argv[0]);
        return 1;
    }
    int k = atoi(argv[1]), n = atoi(argv[2]), trials = atoi(argv[3]);
    if (k <= 0 || n < k || n > (int)PRIME || n > MAX_N)
        die("invalid k or n\n");

    int t = (n - k) / 2;          
    srand(2025);

    uint32_t *msg = malloc(k*sizeof *msg);
    uint32_t *enc = malloc(n*sizeof *enc);
    uint32_t *rec = malloc(n*sizeof *rec);
    uint32_t *dec = malloc(n*sizeof *dec);
    if(!msg || !enc || !rec || !dec) die("oom\n");

    for (int tr = 0; tr < trials; ++tr) {
        for (int i = 0; i < k; ++i) msg[i] = urand32();
        rs_encode(msg, k, n, enc);
        memcpy(rec, enc, n*sizeof *rec);
        int inj = rand() % (t + 1);
        inject_errors(rec, n, inj);
        int fixed = rs_decode(n, k, rec, dec);
        if (fixed != inj)                   die("bad count\n");
        if (memcmp(dec, enc, n*sizeof *dec)) die("wrong symbol\n");
    }
    puts("decoder round-trip ✓");
    return 0;
}
