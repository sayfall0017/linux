.PHONY:all
all: udpclient udpserver

udpclient:Udpclient.cpp
	g++ -o $@ $^ -std=c++17
udpserver:Udpserver.cpp
	g++ -o $@ $^ -std=c++17

.PHONY:clean
clean:
	rm -f udpclient udpserver