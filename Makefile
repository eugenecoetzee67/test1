all: main.cpp
	g++ -std=c++0x -Wall -g main3.cpp -o test1 -lpthread

clean:
	$(RM) test1
