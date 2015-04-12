all: main.cpp
	g++ -std=c++0x -Wall -g main.cpp -o test1 -lpthread

clean:
	$(RM) test1
