//
// Created by ivan on 9/10/18.
//

#ifndef HW_1_REQUEST_H
#define HW_1_REQUEST_H

#include <cstdlib>
#include <boost/asio.hpp>
#include "Response.h"

#define GET "GET"
#define HEAD "HEAD"
#define SKIP_SIZE 256

using std::string;

class Request {
private:

    string method;
    string url;
    string version;
    string encoding;
    string contentLength;
    string rootDir;
    Response response;

public:

    Request(string);
    bool checkMethod();
    void parseRequest(string, size_t, std::function<void (const string&)>, std::function<void (int, size_t)>);

};


#endif //HW_1_REQUEST_H
