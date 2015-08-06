#include "web_server.h"
#include "request_processor.h"

WebServer::WebServer(unsigned int port_number): port(port_number) {}
    
void WebServer::run()
{
    TCPServer server(port);
    cout << "Starting server on port " << port << endl;

    try {
        server.start();
    } catch(string e) {
        cout << "Error: " << e << endl;
        exit(1);
    }

    cout << "Listening" << endl;

    SocketStream* socket;

    while(1) {
        try {
            socket = server.accept();
        }    
        catch(string e) {
            cout << "Error: " << e << endl;
            continue;
        }

        cout << "Someone connected: " << socket->getClientAddress() << ":" << socket->getClientPort() << endl;

        HttpConnection *connection = new HttpConnection(socket);
        new thread(handleClient, connection);
    }
}

void WebServer::handleClient(HttpConnection *connection)
{
    HttpRequest http_request = connection->getRequest();
    HttpResponse response = RequestProcessor::process(http_request);
    connection->sendResponse(response);
    delete connection; // close socket
}

/*
 * HttpConnection method definitions
 */
HttpConnection::HttpConnection(SocketStream *s) : socket(s) {}
HttpConnection::~HttpConnection()
{
    cout << "Closing connection" << endl;
    delete socket;
}

HttpRequest HttpConnection::getRequest()
{
    HttpRequest http_request;
    try {
        string line = socket->readLine();

        istringstream request(line);
        request >> http_request.method >> http_request.uri >> http_request.version;

        // Read headers until an empty line is met:
        while(1) {
            line = socket->readLine();
            if(line.compare("\r\n") == 0) {
                break; // blank line - end of headers
            }

            // Delete "\r\n":
            size_t line_ending = line.find("\r\n");
            if(line_ending != string::npos) {
                line.resize(line_ending);
            }

            // Split key-value by first ": " occurrence:
            size_t delimiter_index = line.find(": ");
            if(delimiter_index != string::npos) {
                string key = line.substr(0, delimiter_index);
                string value = line.substr(delimiter_index + 2);
                http_request.headers[key] = value;
            } else {
                http_request.headers[line] = string("");
            }
        }
    } catch(int e) {
        cout << "EOF: " << e << endl;
    } catch(string e) {
        cout << "Error: " << e << endl;
    }
    return http_request;
}

HttpConnection& HttpConnection::operator<<(const string& s)
{
    this->socket->send(s);
    return *this;
}

void HttpConnection::sendResponse(HttpResponse& http_response)
{
    *this << http_response.version << " " << http_response.status_code << "\r\n";
    for_each(http_response.headers.begin(), http_response.headers.end(), 
        [=](string line){ *this << line << "\r\n"; });
    *this << "\r\n"; // end of headers
    *this << http_response.body << "\r\n";
}
