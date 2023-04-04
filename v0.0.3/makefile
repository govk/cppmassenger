all: test_server test_client

test_server: test_server.cpp server.cpp server.h global.h
        g++ -std=c++14 -o test_server test_server.cpp server.cpp -lpthread -lmysqlclient

test_client: test_client.cpp client.cpp client.h global.h
        g++ -std=c++14 -o test_client test_client.cpp client.cpp -lpthread
clean:
        rm -f test_server test_client