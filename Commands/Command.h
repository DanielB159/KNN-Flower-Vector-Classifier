#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>

#include "../Network/DefaultIO.h"
#include "../Network/SocketIO.h"
#include "../VectorRelated/stringToVec.h"
#include "../VectorRelated/typeDistances.h"
#include "../VectorRelated/slct.h"
#include "../VectorRelated/vectorType.h"


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
        Command();
        ~Command();
        void setDio(int clientSocket);
        void setDescription(std::string s);
        std::string getDescription();
        //send a message to the client
        void sendToClient(std::string message);
        //get a message from the client
        std::string getFromClient();
        virtual void execute(CLIDetails* cli) = 0;
};

#endif