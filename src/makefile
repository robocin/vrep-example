## OBS: Allways you're going to build any new file you need to generate the .o files first from the cpp file with
## the flag -c and them link the .o files in the last build script
## The @ before the command means it won't be shown

CFLAGS = -I../remoteApi -I../include -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255

OS = $(shell uname -s)
ifeq ($(OS), Linux)
	CFLAGS += -D__linux
else
	CFLAGS += -D__APPLE__
endif

EXTERN_API=../remoteApi/extApi.o ../remoteApi/extApiPlatform.o
LIBS=libs/*.cpp

# build the distribution
all: 
	@make clean
	@make remote_api
	g++ $(CFLAGS) -c simulation_1x1.cpp -o simulacao.o

	@mkdir -p bin
	g++ ${EXTERN_API} simulacao.o -o bin/simulacao -lpthread
	./bin/simulacao

# build the remote API
remote_api:
	@gcc $(CFLAGS) -c ../remoteApi/extApi.c -o ../remoteApi/extApi.o
	@gcc $(CFLAGS) -c ../remoteApi/extApiPlatform.c -o ../remoteApi/extApiPlatform.o

# build the simulation group resources
simulation:
	@g++ $(CFLAGS) -c libs/simulation_group.cpp -o bin/lib_group.o -I../remoteApi/

simtest:
	@echo "\nCleaning everything...\n"
	@make clean
	@echo "\nBuilding remote API\n"
	@make remote_api
	@echo "\nBuilding Classes\n"
	@make simulation
	@echo "\nBuilding test objects\n"
	@g++ $(CFLAGS) -c tests/test_simulation_group.cpp -o bin/simtest.o -I../remoteApi/
	@echo "\nBuilding the test executable\n"
	@g++ ${EXTERN_API} bin/*.o -o bin/simtest -lpthread -I../remoteApi/
	@echo "\nBuild Finished!\n"
	@echo "\nRunning test...\n"
	./bin/simtest

# clean every directory
clean:
	@rm -f bin/*.*
	@rm -f *.o 
	@rm -f ../remoteApi/*.o 