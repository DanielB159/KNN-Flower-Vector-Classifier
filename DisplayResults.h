#ifndef DISPLAY_RESULTS_H
#define DISPLAY_RESULTS_H

#include "Command.h"

/// @brief Send results of classification
class DisplayResults : public Command {
    public:
        DisplayResults(int clientSocket);
        /// @brief execute command
        /// @param cliDetails details for execution
        void execute(CLIDetails* cli);
};

#endif