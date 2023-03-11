#include "networkFunctions.h"

//DEPRECATED!!!
string networkFunctions::sendRecieveFromServer(int sock, string inputString) {
    char data_addr[inputString.length()];
                //copying the input to data_addr
                strcpy(data_addr, inputString.c_str());
                //sending the data to the server
                int sent_bytes = send(sock, data_addr, inputString.length(), 0);
                if (sent_bytes < 0) {
                    cout << "error while sending to the server!" << endl;
                    return "Error";
                }
                char buffer[128];
                int expected_data_size = sizeof(buffer);
                //recieving the answer from the server
                int read_bytes = recv(sock, buffer, expected_data_size, 0);
                //validating that data was recieved
                if (read_bytes == 0) {
                    cout << "the server is closed!" << endl;
                    return "Error";
                }
                if (read_bytes < 0) {
                    cout << "error while recieving from the server!" << endl;
                    return "Error";
                }
                //converting buffer to string
                string output(buffer);
                return output;
                
}

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