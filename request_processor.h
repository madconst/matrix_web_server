#ifndef REQUEST_PROCESSOR_H_
#define REQUEST_PROCESSOR_H_

#include <map>
#include "web_server.h"
#include "matrix.hpp"

class RequestProcessor
{
public:
    static HttpResponse HttpErrorResponse(const string code, const string hint = "");
    static HttpResponse process(const HttpRequest& http_request);
    static map<string, string> parseParams(const string params_string);
    static unsigned int getMatrixSize(const map<string, string>& params);
    static void printRequestDetails(const HttpRequest& http_request);
    static string printMatrix(const Matrix<double>& m, bool print_input_fields);
};

#endif // REQUEST_PROCESSOR_H_