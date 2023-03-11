#ifndef NETWORKFUNCTIONS_H
#define NETWORKFUNCTIONS_H

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
using namespace std;


namespace networkFunctions {
    int createTCPclientSocket(const char* ip_adress, int port_no);
    string sendRecieveFromServer(int sock, string inputString);
}

#endif