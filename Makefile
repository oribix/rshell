
VPATH = ./src/main.cpp

LSPATH = ./src/ls.cpp
LSOUTPUT = ./bin/ls

CXXFLAGS = -ansi -Wall -Werror -pedantic

OUTPUTPATH = ./bin/rshell

all: rshell ls
	mkdir -p ./bin		//mv all bin
	g++ $(CXXFLAGS) $(VPATH) -o $(OUTPUTPATH)
	g++ $(CXXFLAGS) $(LSPATH) -o $(LSOUTPUT) 

rshell: 
	mkdir -p ./bin
	g++ $(CXXFLAGS) $(VPATH) -o $(OUTPUTPATH)

ls:
	mkdir -p ./bin
	g++ $(CXXFLAGS) $(LSPATH) -o $(LSOUTPUT)

clean:
	rm -rf bin
