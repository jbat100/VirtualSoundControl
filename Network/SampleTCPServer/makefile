CC=g++
LDFLAGS=-g
CPPFLAGS=-I/usr/local/include
LDLIBS=-L/usr/local/lib -lboost_system -lboost_thread -lboost_regex

tcp_server: tcp_server.o
	$(CC) $(LDFLAGS) $(LDLIBS) -o tcp_server tcp_server.o

tcp_server.o: sample_test_tcp_server.cpp
	$(CC) $(CPPFLAGS) -o tcp_server.o -c sample_test_tcp_server.cpp -W

clean:
	rm -rf *.o

mrproper: clean
	rm -rf tcp_server

