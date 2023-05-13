#######
# Makefile for threadfun2
#######
CC = gcc
CFLAGS = -O0 -g -Wall

all: pthreadstatcalc sudokuvalidator threadarray tokenid

pthreadstatcalc:
	$(CC) $(CFLAGS) pthreadstatcalc.c -o pthreadstatcalc -lm

sudokuvalidator:
	$(CC) $(CFLAGS) sudokuvalidator.c -o sudokuvalidator

threadarray:
	$(CC) $(CFLAGS) threadarray.c -o threadarray

tokenid:
	$(CC) $(CFLAGS) tokenid.c -o tokenid

clean:
	rm -f *.o *~ sudokuvalidator
	rm -f *.o *~ pthreadstatcalc
	rm -f *.o *~ threadarray
	rm -f *.o *~ tokenid
	rm -rf *.dSYM