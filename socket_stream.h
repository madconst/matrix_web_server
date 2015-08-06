#ifndef SOCKET_STREAM_H_
#define SOCKET_STREAM_H_

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

class SocketStream
{
    int sd;
    string client_address;
    unsigned int client_port;

public:
    friend class TCPServer;

    ~SocketStream();
    ssize_t send(const char* buffer, const size_t len);
    ssize_t send(const string msg);
    ssize_t receive(char* buffer, size_t len);
    string readLine();
    string getClientAddress();
    int getClientPort();

private:
    SocketStream(const int socket_descriptor, const struct sockaddr_in* address);
    SocketStream();
    SocketStream(const SocketStream &stream);
};

#endif // SOCKET_STREAM_H_