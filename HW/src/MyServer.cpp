//
// Created by ivan on 9/4/18.
//

#include "../headers/MyServer.h"
#include "../headers/Connector.h"

using boost::asio::ip::tcp;

MyServer::MyServer(std::map<std::string, std::string>* configurationProperties):
    acceptor(io_service, tcp::endpoint(tcp::v4(), (unsigned short)strtoul((*configurationProperties)["port"].c_str(), nullptr, 10))),
    rootDir((*configurationProperties)["document_root "]), threadCount((size_t)strtoul((*configurationProperties)["cpu_limit "].c_str(), nullptr, 10)),
    threadPool(this->threadCount) {
    try {
        this->acceptor.non_blocking(true);
    }
    catch (boost::system::system_error& e) {
        std::cout << e.what() << std::endl;
    }
}

void* runThread(void* io_service) {
    ((boost::asio::io_service*)io_service)->run();
}

void MyServer::start() {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    std::vector<void*> results(this->threadCount);
    for (std::size_t i = 0; i < this->threadCount; i++) {
//        this->threadPool.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));
        this->threadsId.push_back(pthread_create(&this->threadPool.at(i), &attr, runThread, &io_service));
        this->io_service.post(boost::bind(&MyServer::accept, this));
    }
    std::cout << "The server is running in " << this->threadCount << " threads" << std::endl;
    for (std::size_t i = 0; i < this->threadCount; i++) {
        pthread_join(this->threadPool[i], &results[i]);
    }
}

void MyServer::accept() {
    Connector::connectorPointer newConnector(new Connector(this->io_service, this->rootDir));
    acceptor.async_accept(
            newConnector->getSocket(),
            boost::bind(&MyServer::handleConnect, this, newConnector, boost::asio::placeholders::error)
    );
}

void MyServer::handleConnect(Connector::connectorPointer newConnection, const boost::system::error_code &error) {
    if (!this->acceptor.is_open()) {
        return;
    }
    if (!error) {
        newConnection->connect();
    }
    this->accept();
}