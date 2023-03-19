#ifndef UPLOAD_FILE_H
#define UPLOAD_FILE_H
#include "Command.h"

/// @brief Command 1, upload files for classification.
class UploadFile : public Command {
    private:
        const std::string ENTER_ADRESS_OF_TRAIN_VECTORS_MESSAGE = "Please upload your local train csv file.";
        const std::string ENTER_ADRESS_OF_TEST_VECTORS_MESSAGE = "Please upload your local test csv file.";
    public:
        UploadFile(int clientSocket);
        /// @brief Get vectors and parse them.
        /// @param areClassified classified vectors or not
        /// @param cliDetails details for classifiaction
        void readVectors(bool areClassified, CLIDetails* cliDetails);
        /// @brief execute command
        /// @param cliDetails details for execution
        void execute(CLIDetails* cliDetails);
    };

#endif