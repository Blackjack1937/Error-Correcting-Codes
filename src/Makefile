CC      = gcc
CFLAGS  = -O2 -std=c99

all: rs_encoder rs_decoder

rs_encoder.o: rs_encoder.c rs_field.h rs_encoder.h
	$(CC) $(CFLAGS) -c $<

rs_encoder: rs_encoder.c rs_field.h rs_encoder.h
	$(CC) $(CFLAGS) -DRS_ENCODER -o $@ $<

rs_decoder: rs_decoder.c rs_field.h rs_encoder.h rs_encoder.o
	$(CC) $(CFLAGS) -o $@ rs_decoder.c rs_encoder.o

clean:
	rm -f *.o rs_encoder rs_decoder
