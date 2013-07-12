CC = g++

all: registry.h registry_test.h registry_test.cpp
	$(CC) -o registry_test registry_test.cpp

clean: registry_test
	rm registry_test
