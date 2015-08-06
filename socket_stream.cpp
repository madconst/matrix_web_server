#include "socket_stream.h"

SocketStream::~SocketStream()
{
    if(!sd) {
        cout << "Nothing to close" << endl;
        return;
    }
    
    close(sd);
    cout << "TCP socket closed." << endl;
}

ssize_t SocketStream::send(const char* buffer, const size_t len)
{
    return write(sd, buffer, len);
}

ssize_t SocketStream::send(const string msg)
{
    ssize_t len = msg.length();
    return write(sd, msg.c_str(), len);
}

ssize_t SocketStream::receive(char* buffer, size_t len)
{
    return read(sd, buffer, len);
}

string SocketStream::readLine()
{
    ssize_t len;
    const unsigned int max_size = 4096;
    string line_buffer;
    char byte;

    unsigned int bytes_read = 0;

    while(1) {
        len = read(sd, &byte, 1);
        if(len < 0) {
            throw string("Error while reading data");
        }

        if(len == 0) { // EOF
            if(bytes_read == 0) {
                throw -1;
            } else {
                break;
            }
        }

        if(bytes_read < max_size - 1) {
            bytes_read++;
            line_buffer += byte;
        } else {
            throw string("Line too long");
        }

        if(byte == '\n') { // EOL
            break;
        }
    }

    return line_buffer;
}

string SocketStream::getClientAddress()
{
    return client_address;
}

int SocketStream::getClientPort()
{
    return client_port;
}

SocketStream::SocketStream(const int socket_descriptor, const struct sockaddr_in* address) : sd(socket_descriptor)
{
    client_address = inet_ntoa(address->sin_addr);
    client_port = address->sin_port;
}
