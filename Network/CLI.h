#ifndef CLI_H
#define CLI_H

#include <string>
#include <vector>

#include "../VectorRelated/typeDistances.h"
#include "../VectorRelated/vectorFeatures.h"
#include "DefaultIO.h"
#include "../Commands/Command.h"
#include "../Commands/UploadFile.h"
#include "../Commands/AlgorithmSettings.h"
#include "../Commands/ClassifyData.h"
#include "../Commands/DisplayResults.h"
#include "../Commands/DownloadResults.h"

/// @brief This class manages the server functions.
class CLI {
    private:
        CLIDetails details;
        Command* commands[5];
        std::string menu = "Welcome to the KNN Classifier Server. Please choose an option:\n";
        const char* INVALID_INPUT_STRING = "invalid input";
        const int INVALID_INPUT_MSG_LEN = 14;
    public:
        CLI(int clientSocket);
        ~CLI();
        void send_error(int client_sock);
        void setSource(int source);
        int getSource();
        void addVectorType(vectorFeatures::vectorType vec);
        void start();

        CLIDetails getCLIDetails();
};

#endif