#include <iostream>
#include <cstdlib>

#include "web_server.h"

using namespace std;

int main(int argc, char **argv)
{
    if(argc != 2) {
        cout << "Usage: " << argv[0] << " <port_number>" << endl;
        exit(1);
    }

    const unsigned int port = atoi(argv[1]);
    WebServer server(port);
    server.run();

    /* Will never get here */    
    return 0;
}
