//
// Created by ivan on 9/10/18.
//

#include "../headers/Request.h"

Request::Request(string dir): rootDir(dir) {
}

bool Request::checkMethod() {
    return this->method == GET || this->method == HEAD;
}

void Request::parseRequest(string request, size_t size, std::function<void (const string&)> sendHeader, std::function<void (int, size_t)> sendFile) {

    std::istringstream iss(request);
    iss >> this->method;

    if (!checkMethod()) {
        this->response.notAllowed(sendHeader);
        return;
    }

    iss >> this->url >> this->version;
    iss.ignore(SKIP_SIZE, '\n'); // \n after HTTP/1.1
    iss.ignore(SKIP_SIZE, '\n'); // Host
    iss.ignore(SKIP_SIZE, ':');  // Accept-Encoding
    iss >> this->encoding;
    iss.ignore(SKIP_SIZE, '\n'); // \n after identity
    iss.ignore(SKIP_SIZE, ':');  // Content-Length:
    iss >> this->contentLength;
    if (this->method == GET) {
        this->response.get(this->rootDir, this->url, sendHeader, std::move(sendFile), true);
        return;
    } else if (this->method == HEAD) {
        this->response.head(this->rootDir, this->url, sendHeader);
    }

}