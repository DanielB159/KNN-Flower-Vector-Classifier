#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>

#include "DefaultIO.h"
#include "stringToVec.h"
#include "typeDistances.h"
#include "slct.h"
#include "vectorType.h"


/// @brief information shared between commands.
struct CLIDetails {
    int k;
    std::string distanceMetricName;
    Distance* distanceMetricPtr;
    std::vector<vectorFeatures::vectorType> classifiedVectors;
    std::vector<vectorFeatures::vectorType> unclassifiedVectors;
    int source;
};

/// @brief Abstract command class.
class Command {
    private:
        std::string description;
        SocketIO dio;
    
    public:
        Command() : dio(0){}
        ~Command() {}

        void setDio(int clientSocket) {
            SocketIO sio(clientSocket);
            this->dio = sio;
        }
        void setDescription(std::string s) {
            this->description = s;
        }
        std::string getDescription() {
            return this->description;
        }
        //send a message to the client
        void sendToClient(std::string message) {
            dio.write(message);
        }
        //get a message from the client
        std::string getFromClient() {
            return dio.read();
        }
        virtual void execute(CLIDetails* cli) = 0;
};

#endif