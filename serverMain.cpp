#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using std::cout;
using std::cin;
using std::endl;

#include <string>
#include "stringToVec.h"
#include "typeDistances.h"
#include "dataInput.h"
#include "vectorType.h"
#include "slct.h"
#include "DefaultIO.h"
#include "Command.h"
#include "CLI.h"

#include <thread>
#include <map>

using std::string;


const char* INVALID_INPUT_STRING = "invalid input";
const int INVALID_INPUT_MSG_LEN = 14;

/// @brief send error to client.
/// @param client_sock client identifier
void send_error(int client_sock) { 
    // error reading message, send "invalid input"
    int sent_bytes = send(client_sock, INVALID_INPUT_STRING, INVALID_INPUT_MSG_LEN, 0);
    if (sent_bytes < 0) {
        perror("error sending to client");
    }
}

void handle_client(int sock) {
    //create a new CLI that will handle the interaction with this client
    CLI* cli = new CLI(sock);
    cli->start();
    delete(cli);
    
    return;
}

/// @brief main function for server
/// @param argc argument count
/// @param argv arguments [port]
/// @return exit value
int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "please enter valid amount of arguments.\nformat: \"<port>\"" << endl;
        return 0;
    }

    const int server_port = std::stoi(argv[1]);
    //create socket object
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { 
        perror("error creating socket");
    }

    //create and zero out sin for socket information
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    
    //set sin fields
    sin.sin_family = AF_INET;           //tcp server
    sin.sin_addr.s_addr = INADDR_ANY;   //any address
    sin.sin_port = htons(server_port);  //my port

    //bind socket
    if (bind(sock,(struct sockaddr*) &sin, sizeof(sin)) < 0) {
        perror("error binding socket");
    }

    //listen for sockets, TODO: is this a cumulative limit?
    if (listen(sock, 40) < 0) {
        perror("error listening to a socket");
    }

    //listen for new connections indefinitely
    while (true) {
        //allocate client_sin for client information
        struct sockaddr_in client_sin;
        unsigned int addr_len = sizeof(client_sin);

        //accept client
        int client_sock = accept(sock, (struct sockaddr*) &client_sin, &addr_len);
        if (client_sock < 0) {
            perror("error accepting client");
            continue;
        }

        //start a thread for the client
        std::thread client_handler(handle_client, client_sock);
        client_handler.detach();
    }
}