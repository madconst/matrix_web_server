#ifndef WEB_SERVER_H_
#define WEB_SERVER_H_

#include <iostream>
#include <sstream>
#include <thread>
#include <algorithm>
#include <string>
#include <map>

#include "tcp_server.h"
#include "socket_stream.h"

using namespace std;

/*
http_request:
- request (method, url, version)
- headers (header_name: value)
- empty line
- body (optional)
- EOF?
*/

struct HttpRequest
{
    string method, uri, version;
    map<string, string> headers;
};

struct HttpResponse
{
    string version, status_code;
    vector<string> headers;
    string body;
};

class HttpConnection
{
    SocketStream *socket;
public:
    HttpConnection(SocketStream *s);
    ~HttpConnection();
    HttpRequest getRequest();
    void sendResponse(HttpResponse& http_response);
    HttpConnection& operator<<(const string& s);
};

class WebServer
{
    unsigned int port;

public:
    WebServer(unsigned int port_number);
    void run();

private:
    static void handleClient(HttpConnection *connection);
};

#endif // WEB_SERVER_H_