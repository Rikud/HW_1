FROM ubuntu:16.04
MAINTAINER Ivan Nemshilov
RUN apt-get update && apt-get install -y --no-install-recommends apt-utils
RUN apt-get -y install g++
RUN apt-get -y install libboost-all-dev

ADD ./HW/ ./

RUN g++ src/*.cpp -pthread -lboost_thread -lboost_filesystem -std=c++14 -lboost_system -o HW_1

EXPOSE 80

CMD ./HW_1
