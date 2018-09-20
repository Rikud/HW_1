//
// Created by ivan on 9/10/18.
//

#ifndef HW_1_RESPONCE_H
#define HW_1_RESPONCE_H

#include <string>
#include <fstream>

#include <boost/asio.hpp>
/*#include <boost/lexical_cast.hpp>*/
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/fstream.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/progress.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/date_time/time_facet.hpp"

#define HEADER_OK "HTTP/1.1 200 OK\r\n"
#define HEADER_BAD_REQUEST "HTTP/1.1 400 Bad Request\r\n"
#define HEADER_FORBIDDEN "HTTP/1.1 403 Forbidden\r\n"
#define HEADER_NOT_FOUND "HTTP/1.1 404 Not Found\r\n"
#define HEADER_NOT_ALLOWED "HTTP/1.1 405 Method Not Allowed\r\n"
#define HEADER_DATE "Date: "
#define HEADER_SERER "Server: IvanServer/0.1 (Unix)"
#define HEADER_CONNECTION "Connection: close\r\n"
#define HEADER_CONTENT_LENGTH "Content-Length: "
#define CONTENT_TYPE_HTML "Content-Type: text/html\r\n"
#define CONTENT_TYPE_CSS "Content-Type: text/css\r\n"
#define CONTENT_TYPE_GIF "Content-Type: image/gif\r\n"
#define CONTENT_TYPE_JPEG "Content-Type: image/jpeg\r\n"
#define CONTENT_TYPE_JS "Content-Type: application/javascript\r\n"
#define CONTENT_TYPE_PNG "Content-Type: image/png\r\n"
#define CONTENT_TYPE_SWF "Content-Type: application/x-shockwave-flash\r\n"

using std::string;

//The class responsible for reading the configuration file
class Response {
private:
    bool checkRootDir(string, string);

    string typeDefinition(string);

    string mainHeaders();

    void notFound(std::function<void (const string&)>);
    void forbidden(std::function<void (const string&)>);
    void badRequest(std::function<void (const string&)>);

public:
    void notAllowed(std::function<void(const string&)> sendHeader);
    void get(string, string, std::function<void (const string&)>, std::function<void (int, size_t)>, bool);
    void head(string, string, std::function<void (const string&)>);
};


#endif //HW_1_RESPONCE_H
