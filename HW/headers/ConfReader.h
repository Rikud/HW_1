//
// Created by ivan on 9/4/18.
//

#ifndef HW_1_CONFREADER_H
#define HW_1_CONFREADER_H
#include <iostream>
#include <map>
#include <fstream>

#define DEFULT_PORT "80"
#define DEFULT_DOCUMENT_ROOT "/var/www/html"
#define DEFULT_CPU_COUNT "4"
#define CONFIGURATION_PATH "/etc/httpd.conf"

#define READ_BUFF_LENGTH 256

class ConfReader {

private:

    std::map<std::string, std::string> configurationProperties;

    void parseAndSavePropertyString(std::string);

public:
    ConfReader();
    ~ConfReader() = default;
    bool readConf();
    std::map<std::string, std::string>* getConfProp();

};


#endif //HW_1_CONFREADER_H
