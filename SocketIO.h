#ifndef SOCKET_IO_H
#define SOCKET_IO_H

#include "DefaultIO.h"

/// @brief DefaultIO for dealing with sockets.
class SocketIO : public DefaultIO {
    private:
        int clientSocket;
        const char* INVALID_INPUT_STRING = "invalid input";
        const int INVALID_INPUT_MSG_LEN = 14;
    public:
        SocketIO(int clientSocketDescriptor);
        void operator=(const SocketIO other);
        /// @brief Send error to client
        /// @param client_sock client's socket
        void send_error(int client_sock);
        /// @brief Write to socket
        /// @param s s string to write
        void write(std::string s);

        /// @brief Read from socket.
        /// @return message read
        std::string read();
};

#endif