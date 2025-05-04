#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "rs_field.h" 
//prime-field ops
void rs_encode(const uint32_t*,int,int,uint32_t*);

static uint32_t urand32(void){ return (uint32_t)(rand() % PRIME); }

static size_t file_to_symbols(const char *path,uint32_t **out)
{
    FILE *f = fopen(path,"rb");
    if(!f){ perror(path); exit(1); }
    fseek(f,0,SEEK_END); long len = ftell(f); rewind(f);

    uint8_t *buf = malloc(len);
    fread(buf,1,len,f); fclose(f);

    *out = malloc(len*sizeof(uint32_t));
    for(long i=0;i<len;i++) (*out)[i]=buf[i];
    free(buf);
    return (size_t)len;            
}
static void add_noise(uint32_t *v,int n,double p)
{
    for(int i=0;i<n;i++)
        if((double)rand()/RAND_MAX < p){
            uint32_t neo;
            do neo = urand32(); while(neo==v[i]);
            v[i]=neo;
        }
}

int main(int argc,char**argv)
{
    if(argc!=5){
        fprintf(stderr,"usage: %s infile k n p\n",argv[0]); return 1;}
    const char *infile = argv[1];
    int k = atoi(argv[2]), n = atoi(argv[3]);
    double p = atof(argv[4]);
    if(k<=0||n<k||n>PRIME) {fputs("bad k,n\n",stderr); return 1;}


    srand((unsigned)time(NULL));
    uint32_t *payload; size_t N = file_to_symbols(infile,&payload);
    size_t blocks = (N + k - 1) / k;
    uint32_t *code  = malloc(n*sizeof(uint32_t));
    uint32_t *noisy = malloc(n*sizeof(uint32_t));
    printf("File \"%s\"  (%zu symbols)\n", infile, N);
    printf("RS(%d,%d) over 𝔽₆₅₅₃₇ , correction capacity t = %d\n",
           n,k,(n-k)/2);
    printf("Channel error-rate p = %.1f %%\n\n", p*100);

    size_t idx=0; int total_flips=0;
    for(size_t b=0;b<blocks;b++){
        uint32_t msg[k];
        for(int i=0;i<k;i++)
            msg[i] = (idx < N ? payload[idx++] : 0);   //zero-pad

        rs_encode(msg,k,n,code);  
        memcpy(noisy,code,n*sizeof(uint32_t));
        add_noise(noisy,n,p);       
        int flips=0;
        for(int i=0;i<n;i++) if(code[i]!=noisy[i]) ++flips;
        total_flips += flips;

        printf("blk %3zu : errors = %2d / %d\n", b+1, flips,n);
    }

    printf("\nAverage symbol errors per block: %.2f\n",
           (double)total_flips/blocks);
    printf("\nEmpirical p  = %.4f\n",
        (double)total_flips / (blocks * n));
 
    free(payload); free(code); free(noisy);
    return 0;
}
