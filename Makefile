CC = g++
CFLAGS = -g -std=c++11
LIBCV = `pkg-config --libs --cflags opencv`

all: filter_difference.o filter_smooth.o filter_basic.o matrix_conv.o expect.o matrix_math.o color_histogram.o color_colorspace.o morphology_basic.o color_interpolate.o
	$(CC) $(CFLAGS) -o target main.cpp $^ $(LIBCV)

test_all: matrix expect.o
	$(CC) $(CFLAGS) -o target ./test/test_all.cpp matrix_math.o $(LIBCV)

test_matrix: $(patsubst ./matrix/%.cpp, %.o, $(wildcard ./matrix/*.cpp)) expect.o
	$(CC) $(CFLAGS) -o target ./test/test_matrix.cpp $^ $(LIBCV)

test_filter: $(patsubst ./filter/%.cpp, %.o, $(wildcard ./filter/*.cpp)) matrix_math.o expect.o
	$(CC) $(CFLAGS) -o target ./test/test_filter.cpp $^ $(LIBCV)

matrix_%.o: ./matrix/matrix_%.cpp
	$(CC) $(CFLAGS) -o $@ -c $< $(LIBCV)

filter_%.o: ./filter/filter_%.cpp $(patsubst ./matrix/%.cpp, %.o, $(wildcard ./matrix/*.cpp)) expect.o
	$(CC) $(CFLAGS) -o $@ -c $< $(LIBCV)

color_%.o: ./color/color_%.cpp $(patsubst ./matrix/%.cpp, %.o, $(wildcard ./matrix/*.cpp)) expect.o
	$(CC) $(CFLAGS) -o $@ -c $< $(LIBCV)

morphology_%.o: ./morphology/morphology_%.cpp $(patsubst ./matrix/%.cpp, %.o, $(wildcard ./matrix/*.cpp)) expect.o
	$(CC) $(CFLAGS) -o $@ -c $< $(LIBCV)

expect.o: 
	$(CC) $(CFLAGS) -o expect.o -c ./expect/expect.cpp 

clean:
	rm -rf target a.out *.o