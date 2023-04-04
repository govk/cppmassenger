all: test_server test_client
all: test_server1 test_client1

test_server1: test_server1.cpp server1.cpp server1.h global1.h
        g++ -std=c++14 -o test_server1 test_server1.cpp server1.cpp -lpthread -lmysqlclient -lhiredis

test_client1: test_client1.cpp client1.cpp client1.h global1.h
        g++ -std=c++14 -o test_client1 test_client1.cpp client1.cpp -lpthread
        
clean:
        rm -f test_server1 test_client1