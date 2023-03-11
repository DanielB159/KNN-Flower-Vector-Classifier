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
#include "dataInput.h"

//DefaultIO will be the abstract class that every Command uses in order to read and write I/O
class DefaultIO {
    public:
        virtual void write(std::string s) = 0;
        virtual std::string read() = 0;
};


/// @brief DefaultIO for dealing with sockets.
class SocketIO : public DefaultIO {
    private:
        int clientSocket;
        const char* INVALID_INPUT_STRING = "invalid input";
        const int INVALID_INPUT_MSG_LEN = 14;
    public:
        SocketIO(int clientSocketDescriptor) {
            this->clientSocket = clientSocketDescriptor;
        }

        void operator=(const SocketIO other) {
            this->clientSocket = other.clientSocket;
        }

        /// @brief Send error to client
        /// @param client_sock client's socket
        void send_error(int client_sock) { 
            // error reading message, send "invalid input"
            int sent_bytes = send(client_sock, INVALID_INPUT_STRING, INVALID_INPUT_MSG_LEN, 0);
            if (sent_bytes < 0) {
                perror("error sending to client");
            }
        }

        /// @brief Write to socket
        /// @param s s string to write
        void write(std::string s) {
            int sent_bytes = send(clientSocket, (void *)s.c_str(), s.length() + 1, 0);
            if (sent_bytes < 0) {
                perror("error sending to client");
            }
        }

        /// @brief Read from socket.
        /// @return message read
        std::string read() {
            char buffer[4096];
            int expected_data_len = sizeof(buffer);
            //receive message
            int read_bytes = recv(clientSocket, buffer, expected_data_len, 0);
            
            //std::cout << "socketio, bytes:" << std::to_string(read_bytes) << std::endl << buffer << std::endl;
            
            if (read_bytes == 0) {
                // connection is closed 
                return "connection closed";
            } else if (read_bytes < 0) {
                send_error(clientSocket);
                return "error whole recieving from client";
            } else {
                std::string answer(buffer);
                return answer;
            }
        } 
};

#endif