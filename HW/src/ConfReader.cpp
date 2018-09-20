//
// Created by ivan on 9/4/18.
//

#include "../headers/ConfReader.h"

ConfReader::ConfReader() {
    this->configurationProperties["port"] = std::string(DEFULT_PORT);
    this->configurationProperties["document_root "] = std::string(DEFULT_DOCUMENT_ROOT);
    this->configurationProperties["cpu_limit "] = std::string(DEFULT_CPU_COUNT);
}

std::map<std::string, std::string>* ConfReader::getConfProp() {
    return &this->configurationProperties;
};

bool ConfReader::readConf() {
    try {
        std::ifstream confFile(CONFIGURATION_PATH);

        if (confFile.is_open()) {
            char buf[READ_BUFF_LENGTH];

            for (size_t i = 0; confFile.good(); ++i) {
                confFile.getline(buf, READ_BUFF_LENGTH);
                parseAndSavePropertyString(buf);
            }
            confFile.close();
        }

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

void ConfReader::parseAndSavePropertyString(std::string stringForParse) {
    size_t spacePosition_1 = stringForParse.find_first_of(' ');
    std::string property = stringForParse.substr(0, spacePosition_1 + 1);
    size_t spacePosition_2 = stringForParse.find_first_of(' ', spacePosition_1 + 1);
    std::string value = stringForParse.substr(spacePosition_1 + 1, spacePosition_2 - spacePosition_1 - 1);
    this->configurationProperties[property] = value;
}