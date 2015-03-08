SRC = ./src/
OUT = ./bin/

CXXFLAGS = -ansi -Wall -Werror -pedantic


all: rshell ls cp

rshell: bin 
	g++ $(CXXFLAGS) $(SRC)main.cpp -o $(OUT)rshell

ls: bin
	g++ $(CXXFLAGS) $(SRC)ls.cpp -o $(OUT)ls

cp: bin
	g++ $(CXXFLAGS) $(SRC)cp.cpp -o $(OUT)cp

bin:
	mkdir -p ./bin

clean:
	rm -rf bin
