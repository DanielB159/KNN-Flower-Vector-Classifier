#ifndef ALGORITHM_SETTINGS_H
#define ALGORITHM_SETTINGS_H
#include "Command.h"

/// @brief Command 2, set settings for classifiaction.
class AlgorithmSettings : public Command {
    private:
        const std::string INVALID_INPUT_METRIC = "invalid value for metric";
        const std::string INVALID_INPUT_K = "invalid value for K";
    public:
        AlgorithmSettings(int clientSocket);
        /// @brief execute command
        /// @param cliDetails details for execution
        void execute(CLIDetails* cli);
};

#endif