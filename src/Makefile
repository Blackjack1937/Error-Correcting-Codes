CC      = gcc
CFLAGS  = -O2 -std=c99

all: rs_encoder rs_decoder rs_encoder_test rs_decoder_test

rs_encoder.o: rs_encoder.c rs_field.h rs_encoder.h
	$(CC) $(CFLAGS) -c $<

rs_encoder: rs_encoder.c rs_field.h rs_encoder.h
	$(CC) $(CFLAGS) -DRS_ENCODER -o $@ $<

rs_encoder_test: rs_encoder_test.c rs_encoder.o rs_field.h
	$(CC) $(CFLAGS) -o $@ rs_encoder_test.c rs_encoder.o
rs_poly.o: rs_poly.c rs_poly.h rs_field.h
	$(CC) $(CFLAGS) -c $<
rs_decoder.o: rs_decoder.c rs_field.h rs_poly.h
	$(CC) $(CFLAGS) -c $<           # ← no -DRS_DECODER_MAIN here

rs_decoder: rs_decoder.c rs_encoder.o rs_poly.o
	$(CC) $(CFLAGS) -DRS_DECODER_MAIN -o $@ $^
	#          ^ adds the driver inside rs_decoder.c
rs_decoder_test: rs_decoder_test.c rs_encoder.o rs_decoder.o rs_poly.o rs_field.h
	$(CC) $(CFLAGS) -o $@ $^
clean:
	rm -f *.o rs_encoder rs_decoder rs_encoder_test rs_decoder_test
