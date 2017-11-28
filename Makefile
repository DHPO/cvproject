CC = g++
CFLAGS = -g -std=c++11
LIBCV = `pkg-config --libs --cflags opencv`

all: matrix_conv.o
	$(CC) $(CFLAGS) -o target main.cpp $^ $(LIBCV)

test_all: matrix expect.o
	$(CC) $(CFLAGS) -o target ./test/test_all.cpp matrix_math.o $(LIBCV)

test_matrix: $(patsubst ./matrix/%.cpp, %.o, $(wildcard ./matrix/*.cpp)) expect.o
	$(CC) $(CFLAGS) -o target ./test/test_matrix.cpp $^ $(LIBCV)

matrix_%.o: ./matrix/matrix_%.cpp
	$(CC) $(CFLAGS) -o $@ -c $< $(LIBCV)

expect.o: 
	$(CC) $(CFLAGS) -o expect.o -c ./expect/expect.cpp 

clean:
	rm -rf target a.out *.o