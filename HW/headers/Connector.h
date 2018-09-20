//
// Created by ivan on 9/5/18.
//

#ifndef HW_1_CONNECTOR_H
#define HW_1_CONNECTOR_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <sys/sendfile.h>
#include <iostream>

#include "Request.h"
#include "Response.h"

using boost::asio::ip::tcp;

class Connector: public boost::enable_shared_from_this<Connector> {
private:

    tcp::socket connectorSocket;
    static const size_t bufferSize = 1024;
    char buffer[bufferSize];
    Request request;
    Response responce;
    off_t offset;
    static const size_t sizeOfFilePart = 16384; // 2^14 16 kb

    void read(const boost::system::error_code &error, size_t size);
    void write(const boost::system::error_code& error, std::size_t bytes_transferred);
    void sendMessage(const std::string &message);
    void sendFile(int fd, size_t size);
    void disconnect();

public:

    typedef boost::shared_ptr<Connector> connectorPointer;

    Connector(boost::asio::io_service &, string);
    ~Connector();

    tcp::socket& getSocket() {
        return connectorSocket;
    };

    void connect();

};


#endif //HW_1_CONNECTOR_H
