CXXFLAGS = -std=c++20 -g -I. -O3

all: main main_simpl

main: main.cpp
	g++ $(CXXFLAGS) -o main main.cpp

main_simpl: main_simpl.cpp
	g++ $(CXXFLAGS) -o main_simpl main_simpl.cpp

compile_commands.json: Makefile
	bear -- make

.PHONY: clean
clean:
	rm -f main main_simpl compile_commands.json
