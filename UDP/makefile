.PHONY:all clean
all: updclient updserver

updclient : Updclient.cc
	g++ -o @$ @^ -std=c++17
updserver : Updserver.cc
	g++ -o @$ @^ -std=c++17

clean:
	rm -f updclient updserver