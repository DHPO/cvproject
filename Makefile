CC = g++
CFLAGS = -g -std=c++11
LIBCV = `pkg-config --libs --cflags opencv`

all: 
	$(CC) $(CFLAGS) -o target main.cpp $(LIBCV)

test_all: matrix
	$(CC) $(CFLAGS) -o target ./test/test_all.cpp matrix_math.o $(LIBCV)

test_matrix_math: matrix
	$(CC) $(CFLAGS) -o target ./test/test_matrix_math.cpp matrix_math.o $(LIBCV)

matrix:
	$(CC) $(CFLAGS) -o matrix_math.o -c ./Matrix/matrix_math.cpp $(LIBCV)

clean:
	rm -rf target a.out *.o