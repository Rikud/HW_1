//
// Created by ivan on 9/4/18.
//

#ifndef HW_1_MYSERVER_H
#define HW_1_MYSERVER_H


#include <string>
#include <map>
#include <pthread.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "Connector.h"

//Main server class
class MyServer {
private:

    //io_service предоставляет основные функции ввода-вывода для сетевых сокетов
    boost::asio::io_service io_service;
    //acceptor используется для приема новых соединений сокетов
    tcp::acceptor acceptor;
    //количество нитей
    size_t threadCount;
    std::string rootDir;

    std::vector<int> threadsId;
    std::vector<pthread_t> threadPool;

    void accept();

public:

    explicit MyServer(std::map<std::string, std::string>* configurationProperties);
    ~MyServer() = default;
    void start();
    void handleConnect(Connector::connectorPointer newConnection, const boost::system::error_code &error);

};


#endif //HW_1_MYSERVER_H
