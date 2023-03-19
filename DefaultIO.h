#ifndef DEFAULTIO_H
#define DEFAULTIO_H

#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

//DefaultIO will be the abstract class that every Command uses in order to read and write I/O
class DefaultIO {
    public:
        virtual void write(std::string s) = 0;
        virtual std::string read() = 0;
};

#endif