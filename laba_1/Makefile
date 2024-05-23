CC = g++
CFLAGS = -std=c++11

all: float double

float:
	$(CC) $(CFLAGS) -DARRAY_TYPE=float -o program_float 1.cpp

double:
	$(CC) $(CFLAGS) -DARRAY_TYPE=double -o program_double 1.cpp

clean:
	rm -f program_float program_double
