#ifndef CLASSIFY_DATA_H
#define CLASSIFY_DATA_H
#include "Command.h"


namespace slct {
    //creating a Comparator for dataInput::distanceType
    template <>
    class Comparator<vectorFeatures::distanceType> {
        public:
            bool operator()(const vectorFeatures::distanceType& obj1, const vectorFeatures::distanceType& obj2) {
                return obj1.distance < obj2.distance;
            }
    };
}

/// @brief classify data in CLIDetails
class ClassifyData : public Command {
    public:
        ClassifyData(int clientSocket);
        /// @brief execute command
        /// @param cliDetails details for execution
        void execute(CLIDetails* cli);
};


#endif