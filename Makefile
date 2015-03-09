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

Vall: Vrshell Vls Vcp bin

Vrshell: bin 
	g++ -g $(CXXFLAGS) $(SRC)main.cpp -o $(OUT)rshell

Vls: bin
	g++ -g $(CXXFLAGS) $(SRC)ls.cpp -o $(OUT)ls

Vcp: bin
	g++ -g $(CXXFLAGS) $(SRC)cp.cpp -o $(OUT)cp

clean:
	rm -rf bin
