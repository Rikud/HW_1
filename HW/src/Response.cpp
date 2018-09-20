//
// Created by ivan on 9/10/18.
//

#include "../headers/Response.h"

namespace fileSystem = boost::filesystem;

bool urlDecode(const std::string &in, std::string &out) {
    out.clear();
    for (std::size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '%') {
            if (i + 3 <= in.size()) {
                int value = 0;
                std::istringstream is(in.substr(i + 1, 2));
                if (is >> std::hex >> value) {
                    out += static_cast<char>(value);
                    i += 2;
                }
                else {
                    return false;
                }
            }
            else {
                return false;
            }
        }
        else if (in[i] == '+') {
            out += ' ';
        }
        else {
            out += in[i];
        }
    }
    return true;
}

string removeQuery(const std::string &path) {
    std::size_t found = path.find_last_of('?');
    string withoutQuery = path.substr(0, found);
    return withoutQuery;
}

const std::string currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%a, %d %B %Y %X GMT", &tstruct);

    return buf;
}

void Response::head(string rootDir, string originalPath, std::function<void (const string&)> sendHeader) {
    get(rootDir, originalPath, sendHeader, nullptr, false);
}

void Response::get(string rootDir, string originalPath, std::function<void (const string&)> sendHeader, std::function<void (int, size_t)> sendFile, bool isSendFile) {

    string path, pathWithoutQuery;
    pathWithoutQuery = removeQuery(originalPath);

    if (!urlDecode(pathWithoutQuery, path)) {
        badRequest(sendHeader); // wrong url
    }

    fileSystem::path relativePath(rootDir + path);
    bool dirExist = false;
    if (is_directory(relativePath)) { // Directory index file name index.html
        relativePath /= "index.html";
        dirExist = true;
    }

    boost::system::error_code ec;
    fileSystem::path absolutePath = canonical(relativePath, rootDir, ec);
    if (ec) {
        if (dirExist) {
            forbidden(sendHeader); // this dir exist but file index.html does not exist
        } else {
/*            std::cout << "relativePath" << std::endl;
            std::cout << relativePath << std::endl;
            std::cout << "rootDir" << std::endl;
            std::cout << rootDir << std::endl << std::endl;*/
            notFound(sendHeader); // this file/dir does not exist
        }
        return;
    }

    if(!checkRootDir(absolutePath.string(), rootDir)) {
/*        std::cout << "absolutePath" << std::endl;
        std::cout << absolutePath.string() << std::endl;
        std::cout << "rootDir" << std::endl;
        std::cout << rootDir << std::endl << std::endl;*/
        notFound(sendHeader); // this file over the root_path
        return;
    }

    boost::uintmax_t filesize = file_size(absolutePath, ec);

    string contentType = typeDefinition(absolutePath.string());
    string length = HEADER_CONTENT_LENGTH + std::to_string(filesize);

    sendHeader(HEADER_OK + mainHeaders() + contentType + length + "\r\n\r\n");

    if (isSendFile) {
        int fd = open(absolutePath.c_str(), O_RDONLY);
        sendFile(fd, filesize);
        close(fd);
    }
}

bool Response::checkRootDir(string rootDir, string path) {
    return boost::algorithm::contains(rootDir, path);
}

string Response::typeDefinition(string file) {
    std::size_t found = file.find_last_of('.');

    string type = file.substr(found + 1);

    if (type == string("html")) {
        return CONTENT_TYPE_HTML;
    }

    if (type == string("txt")) {
        return CONTENT_TYPE_HTML;
    }

    if (type == string("css")) {
        return CONTENT_TYPE_CSS;
    }

    if (type == string("gif")) {
        return CONTENT_TYPE_GIF;
    }

    if (type == string("jpeg")) {
        return CONTENT_TYPE_JPEG;
    }

    if (type == string("jpg")) {
        return CONTENT_TYPE_JPEG;
    }

    if (type == string("js")) {
        return CONTENT_TYPE_JS;
    }

    if (type == string("png")) {
        return CONTENT_TYPE_PNG;
    }

    if (type == string("swf")) {
        return CONTENT_TYPE_SWF;
    }

    return CONTENT_TYPE_HTML;
}

void Response::notFound(std::function<void(const string&)> sendHeader) {
    sendHeader(HEADER_NOT_FOUND + mainHeaders());
}

void Response::notAllowed(std::function<void(const string&)> sendHeader) {
    sendHeader(HEADER_NOT_ALLOWED + mainHeaders());
}

void Response::forbidden(std::function<void(const string &)> sendHeader) {
    sendHeader(HEADER_FORBIDDEN + mainHeaders());
}

void Response::badRequest(std::function<void(const string &)> sendHeader) {
    sendHeader(HEADER_BAD_REQUEST + mainHeaders());
}

string Response::mainHeaders() {
    string currentDate = HEADER_DATE + currentDateTime() + "\r\n";
    return currentDate + HEADER_SERER + HEADER_CONNECTION;
}