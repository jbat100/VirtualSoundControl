CC=g++
LDFLAGS=-g
CPPFLAGS=-I/usr/local/include
LDLIBS=-L/usr/local/lib -lboost_system -lboost_thread -lboost_regex

tcp_client: tcp_client.o
	$(CC) $(LDFLAGS) $(LDLIBS) -o tcp_client tcp_client.o

tcp_client.o: sample_test_tcp_client.cpp
	$(CC) $(CPPFLAGS) -o tcp_client.o -c sample_test_tcp_client.cpp -W

clean:
	rm -rf *.o

mrproper: clean
	rm -rf tcp_client
