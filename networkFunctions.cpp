#include "networkFunctions.h"

/// @brief Create TCP client socket
/// @param ip_adress ip address of server
/// @param port_no port of server
/// @return socket identifier
int networkFunctions::createTCPclientSocket(const char* ip_adress, int port_no) {
        int sock = socket(AF_INET, SOCK_STREAM , 0);
        if (sock < 0) {
            perror("There was an error creating the socket!");
            return sock;
        }
        //connecting via TCP protocol to server using the created socket
        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = inet_addr(ip_adress);
        sin.sin_port = htons(port_no);
        if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
            perror("There was an error connecting to the server!");
            
            return -1;
        }
        return sock;
    }