//
// Created by ivan on 9/5/18.
//

#include "../headers/Connector.h"

Connector::Connector(boost::asio::io_service &io_service, string rootDir): connectorSocket(io_service), request(rootDir), offset(0) {
}

Connector::~Connector() {
    this->disconnect();
}

void Connector::disconnect() {
    if (this->connectorSocket.is_open()) {
        this->connectorSocket.close();
    }
}

void Connector::connect() {
    this->connectorSocket.non_blocking(true);
    this->connectorSocket.native_non_blocking(true);

    this->connectorSocket.async_read_some(
            boost::asio::buffer(this->buffer, this->bufferSize),
            boost::bind(&Connector::read, shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred)
    );
}

void Connector::read(const boost::system::error_code &error, size_t size) {
    if (error) {
        this->disconnect();
        return;
    }

    this->request.parseRequest(std::string(buffer), size,
          std::bind(&Connector::sendMessage, shared_from_this(), std::placeholders::_1),
          std::bind(&Connector::sendFile, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void Connector::write(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (error) {
        this->disconnect();
        return;
    }
}

void Connector::sendMessage(const std::string &message) {

    this->connectorSocket.async_write_some(
            boost::asio::buffer(message, message.size()),
            boost::bind(&Connector::write, shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred)

    );
}

void Connector::sendFile(int fd, size_t size) {
    ssize_t result = 0;
    while (offset < size) {
        result = sendfile(this->connectorSocket.native_handle(), fd, &this->offset, this->sizeOfFilePart);
        if (result < 0) {
            std::cerr << "error: " << errno << std::endl;
            if (errno == 32 || errno == 104) {
                this->disconnect();
                return;
            }
        }
    }
}