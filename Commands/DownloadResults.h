#ifndef DOWNLOAD_RESULTS_H
#define DOWNLOAD_RESULTS_H
#include "Command.h"

/// @brief command 5, send results to be saved into a file.
class DownloadResults : public Command {
    public:
        DownloadResults(int clientSocket);
        /// @brief execute command
        /// @param cliDetails details for execution
        void execute(CLIDetails* cli);
};

#endif