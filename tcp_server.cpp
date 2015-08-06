#include "tcp_server.h"

TCPServer::TCPServer(const unsigned int port) : sd(0), port(port) {}
TCPServer::~TCPServer()
{
	if(sd) {
		close(sd);
	}
}

void TCPServer::start()
{
	if(sd > 0) {
		throw string("Socket already exists");
	}

	sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in address;

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	if(sd < 0) {
		throw string("Failed to create socket");
	}

	if(bind(sd, (struct sockaddr*) &address, sizeof(address)) < 0) {
		throw string("Failed to bind socket");
	}

	if(listen(sd, 5) < 0) {
		throw string("Failed to start listening");
	}
}

SocketStream* TCPServer::accept()
{
	if(!sd) {
		throw string("Socket doesn't exist");
	}

	struct sockaddr_in address;
	socklen_t len = sizeof(address);

	int client_sd = ::accept(sd, (struct sockaddr*) &address, &len);
	if(client_sd < 0) {
		throw string("Failed to accept connection");
	}

	return new SocketStream(client_sd, &address);
}
