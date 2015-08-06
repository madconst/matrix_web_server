#include <stdexcept>
#include "request_processor.h"

HttpResponse RequestProcessor::HttpErrorResponse(const string code, const string hint)
{
    HttpResponse http_response;
    http_response.version = string("HTTP/1.1");
    http_response.status_code = code;
    http_response.body = hint;
    return http_response;
}

HttpResponse RequestProcessor::process(const HttpRequest& http_request)
{

    printRequestDetails(http_request);

    if(http_request.method.compare("GET") != 0) {
        return HttpErrorResponse("501 Not Implemented");
    }

    // Get URL params:
    size_t params_index = http_request.uri.find('?');
    // map<string, string> params;
    string page, params;
    if(params_index != string::npos) {
        page = http_request.uri.substr(0, params_index);
        params = http_request.uri.substr(params_index + 1);
    } else {
        page = http_request.uri;
        params = string("");
    }

    auto params_map = RequestProcessor::parseParams(params);
    const unsigned int default_size = 5;
    unsigned int size = default_size;

    HttpResponse http_response;
    http_response.version = string("HTTP/1.1");
    http_response.status_code = string("200 OK");

    stringstream body;
    body << "<html><body>"
         << "<h1>Welcome to Matrix Transposer&#8482;</h1>"
         << "<style>"
         << "input[type='text'] { border: none; } "
         << "table { border-collapse: collapse; } "
         << "td { border: solid 1px black; padding-left: 10px; padding-right: 10px; }"
         << "</style>";

    HttpResponse http_response303;
    http_response303.version = "HTTP/1.1";
    http_response303.status_code = "303 see other";
    http_response303.headers.push_back("Location: /matrix?n=5");

    if(page.compare("/") == 0 || page.compare("/matrix") == 0) {
        try {
            size = stod(params_map["n"]);
        } catch (invalid_argument e) {
            return http_response303;
        }

        Matrix<double> m(size);

        body << "<form action='transpose'>";
        body << "<input type='hidden' name='n' value='" << size << "'/>";
        body << printMatrix(m, true); // print input fields
        body << "<button type='submit'>Submit</button>";
        body << "</form>";

    } else if(page.compare("/transpose") == 0) {
        try {
            size = stoi(params_map["n"]);
        } catch (invalid_argument e) {
            return http_response303;
        }

        Matrix<double> m(size);
        for(unsigned int i = 0; i < size; i++) {
            auto row = m.getRow(i);
            for(unsigned int j = 0; j < size; j++) {
                string key = string("r") + to_string(i) + string("c") + to_string(j);
                row[j] = stoi(params_map[key]);
            }
        }

        m.transpose();
        body << printMatrix(m, false); // print values
        body << "<a href='/'>Back</a>";
        
    } else {
        return HttpErrorResponse("404 NOT FOUND", "<h1>Page not found</h1> <h2>No such page, dude :(</h2>");
    }

    body << "</body></html>";
    http_response.body += body.str();
    return http_response;
}

map<string, string> RequestProcessor::parseParams(const string params_string)
{
    map<string, string> params;
    istringstream params_stream(params_string);
    for(string token; getline(params_stream, token, '&');) {
        size_t value_index = token.find('=');
        if(value_index != string::npos) {
            params[token.substr(0, value_index)] = token.substr(value_index + 1);
        } else {
            params[token] = "";
        }
    }
    return params;
}

void RequestProcessor::printRequestDetails(const HttpRequest& http_request)
{
    cout << "Method:\t" << http_request.method << endl;
    cout << "URI:\t" << http_request.uri << endl;
    cout << "HTTP version:\t" << http_request.version << endl;
    cout << "Headers:" << endl;

    for_each(http_request.headers.begin(), http_request.headers.end(), 
        [](pair<string, string> header){ cout << ">" << header.first << ":\t" << header.second << endl; }
    );
}

string RequestProcessor::printMatrix(const Matrix<double>& m, bool print_input_fields)
{
    stringstream body;

    body << "<table>";

    for(unsigned int i = 0; i < m.getSize(); i++) {
        body << "<tr>";
        auto row = m.getRow(i);
        for(unsigned int j = 0; j < m.getSize(); j++) {
            body << "<td>";
            if(print_input_fields) {
                body << "<input type='text' size='5' name='r" << i << "c" << j << "' value='" << row[j] << "'/>";
            } else {
                body << row[j];
            }
            body << "</td>";
        }
        body << "</tr>";
    }
    body << "</table>";
    return body.str();
}
