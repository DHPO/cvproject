CC = g++
CFLAGS = -g -std=c++11
LIBCV = `pkg-config --libs --cflags opencv`

all: matrix
	$(CC) $(CFLAGS) -o target main.cpp matrix_math.o $(LIBCV)

test_all: matrix_math matrix_map exp
	$(CC) $(CFLAGS) -o target ./test/test_all.cpp matrix_math.o $(LIBCV)

test_matrix_math: matrix_math exp
	$(CC) $(CFLAGS) -o target ./test/test_matrix.cpp matrix_math.o expect.o $(LIBCV)

matrix_math: exp
	$(CC) $(CFLAGS) -o matrix_math.o -c ./Matrix/matrix_math.cpp $(LIBCV)

matrix_map: exp
	$(CC) $(CFLAGS) -o matrix_map.o -c ./Matrix/matrix_map.cpp $(LIBCV)

exp: 
	$(CC) $(CFLAGS) -o expect.o -c ./expect/expect.cpp 

clean:
	rm -rf target a.out *.o