#include <iostream>
#include "../headers/ConfReader.h"
#include "../headers/MyServer.h"

int main() {

    //create an object that reads the configuration file
    ConfReader confReader;
    //reading the configuration file and checking for readability
    if (confReader.readConf()) {
        //sending configuration data to configure the server before starting
        MyServer server(confReader.getConfProp());
        server.start();
    }

    return 0;
}