
CFLAGS = -ansi -Wall -Werror -pedantic

all: src/main.cpp
	g++ main.cpp -o bin/rshell $(CFLAGS)

rshell: src/main.cpp
	g++ main.cpp -o bin/rshell $(CFLAGS)

clean:
	rm -f bin/rshell
