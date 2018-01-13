CFLAGS = -I../remoteApi -I../include -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255

OS = $(shell uname -s)
ifeq ($(OS), Linux)
	CFLAGS += -D__linux
else
	CFLAGS += -D__APPLE__
endif

all: 
	@rm -f bin/*.*
	@rm -f *.o 
	@rm -f ../remoteApi/*.o 
	g++ $(CFLAGS) -c simulation_1x1.cpp -o simulacao.o
	gcc $(CFLAGS) -c ../remoteApi/extApi.c -o ../remoteApi/extApi.o
	gcc $(CFLAGS) -c ../remoteApi/extApiPlatform.c -o ../remoteApi/extApiPlatform.o
	@mkdir -p bin
	g++ ../remoteApi/extApi.o ../remoteApi/extApiPlatform.o simulacao.o -o bin/simulacao -lpthread
	./bin/simulacao

