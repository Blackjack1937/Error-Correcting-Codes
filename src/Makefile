CC      = gcc
CFLAGS  = -O2 -std=c99
GF_FLAGS = -DGF128_ZECH

all: rs_encoder rs_decoder rs_encoder_test rs_decoder_test rs128_encoder_test

rs_encoder.o: rs_encoder.c rs_field.h rs_encoder.h
	$(CC) $(CFLAGS) -c $<

rs_encoder: rs_encoder.c rs_field.h rs_encoder.h
	$(CC) $(CFLAGS) -DRS_ENCODER -o $@ $<

rs_encoder_test: rs_encoder_test.c rs_encoder.o rs_field.h
	$(CC) $(CFLAGS) -o $@ rs_encoder_test.c rs_encoder.o

rs_poly.o: rs_poly.c rs_poly.h rs_field.h
	$(CC) $(CFLAGS) -c $<

rs_decoder.o: rs_decoder.c rs_field.h rs_poly.h
	$(CC) $(CFLAGS) -c $<

rs_decoder: rs_decoder.c rs_encoder.o rs_poly.o
	$(CC) $(CFLAGS) -DRS_DECODER_MAIN -o $@ $^

rs_decoder_test: rs_decoder_test.c rs_encoder.o rs_decoder.o rs_poly.o rs_field.h
	$(CC) $(CFLAGS) -o $@ $^

Galois/f128.o: Galois/f128.c Galois/f128.h
	$(CC) $(CFLAGS) -c -o $@ $<

Galois/f128-zech.o: Galois/f128-zech.c Galois/f128.h Galois/f128-zech.h
	$(CC) $(CFLAGS) -c -o $@ $<

Galois/rs_poly_gf128.o: Galois/rs_poly_gf128.c Galois/gf128.h rs_poly.h
	$(CC) $(CFLAGS) $(GF_FLAGS) -I. -c -o $@ $<

rs128_encoder.o: Galois/rs128_encoder.c Galois/gf128.h Galois/rs_poly_gf128.o
	$(CC) $(CFLAGS) $(GF_FLAGS) -I. -c -o $@ $<

Galois/rs128_encoder_test.o: Galois/rs128_encoder_test.c Galois/gf128.h
	$(CC) $(CFLAGS) $(GF_FLAGS) -I. -c -o $@ $<

rs128_encoder_test: Galois/rs128_encoder_test.o rs128_encoder.o \
                    Galois/rs_poly_gf128.o Galois/f128.o Galois/f128-zech.o
	$(CC) $(CFLAGS) $(GF_FLAGS) -o $@ $^

clean:
	rm -f *.o Galois/*.o rs_encoder rs_decoder rs_encoder_test \
           rs_decoder_test rs128_encoder_test
