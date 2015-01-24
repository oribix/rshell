
VPATH = ./src/main.cpp

CXXFLAGS = -ansi -Wall -Werror -pedantic

OUTPUTPATH = ./bin/rshell

all: 
	mkdir -p ./bin
	g++ $(CXXFLAGS) $(VPATH) -o $(OUTPUTPATH)

rshell: 
	mkdir -p ./bin
	g++ $(CXXFLAGS) $(VPATH) -o $(OUTPUTPATH)

clean:
	rm -rf bin
