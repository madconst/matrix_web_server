#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_

#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "socket_stream.h"

using namespace std;

class TCPServer
{
private:
    int sd, port;
public:
    TCPServer(const unsigned int port);
    ~TCPServer();
    void start();
    SocketStream* accept();
};

#endif // TCP_SERVER_H_