thread: thread.cpp
	gcc thread.cpp -std=c++11 -lpthread -lstdc++ -lm
all: run
run: thread
	./thread