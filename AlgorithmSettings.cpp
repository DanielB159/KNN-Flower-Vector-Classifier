#include "AlgorithmSettings.h"

AlgorithmSettings::AlgorithmSettings(int clientSocket) {
    setDescription("algorithm settings");
    setDio(clientSocket);
};

/// @brief execute command
/// @param cliDetails details for execution
void AlgorithmSettings::execute(CLIDetails* cli) {
    //send to client the current KNN parameters as they are set in the CLIDetails
    std::string currentParams = "The current KNN parameters are: K = " + std::to_string(cli->k) + 
        ", distance metric = " + cli->distanceMetricName;
    sendToClient(currentParams);

    bool validK = true, validMetric = true;

    //get answer from client on how to change them and if to change them
    std::string clientAnswer = getFromClient();
    //if the user does not want to change tham - the sentBack message will be "keepCurrent"
    if (clientAnswer == "keepCurrent") {
        return;
    } else {
        std::string answer = "";
        //otherwise, parse the string and set new K and DistanceMetric
        int spacePos = clientAnswer.find(" ");
        int newKValue;
        //check if the distance metric and k are valid
        if (spacePos == string::npos) {
            //if the clientAnswer does not contain any " ", it is not valid.
            answer += INVALID_INPUT_METRIC + "\n" + INVALID_INPUT_K;
            validK = false;
            validMetric = false;
        } else {
            //if does contain " ", parse it and check for new parameters
            std::string newK = clientAnswer.substr(0, spacePos);
            std::string newDist = clientAnswer.substr(spacePos + 1, clientAnswer.length() - spacePos + 1);
            try {
                newKValue = stoi(newK);
                if (newKValue <= 0) {
                    validK = false;
                }
            } catch (...) {
                validK = false;
            }
            //filter by all of the options of newDist
            if (newDist == "AUC") {
                if (validK) {
                    delete(cli->distanceMetricPtr);
                    cli->distanceMetricName = "AUC";
                    cli->distanceMetricPtr = new EuclidianDist();
                }
            } else if (newDist == "MAN") {
                if (validK) {
                    delete(cli->distanceMetricPtr);
                    cli->distanceMetricName = "MAN";
                    cli ->distanceMetricPtr = new ManhattanDist();
                }
            } else if (newDist == "MIN") {
                if (validK) {
                    delete(cli->distanceMetricPtr);
                    cli->distanceMetricName = "MIN";
                    cli ->distanceMetricPtr = new MinkowskiDist();
                }
            } else if (newDist == "CHB") {
                if (validK) {
                    delete(cli->distanceMetricPtr);
                    cli->distanceMetricName = "CHB";
                    cli ->distanceMetricPtr = new ChebyshevDist();
                }
            } else if (newDist == "CAN") {
                if (validK) {
                    delete(cli->distanceMetricPtr);
                    cli->distanceMetricName = "CAN";
                    cli ->distanceMetricPtr = new CanberraDist();
                }
            } else {
                validMetric = false;
            }


            if (!validMetric && validK) {
                answer += INVALID_INPUT_METRIC;
            }
            else if (!validK && validMetric) {
                answer += INVALID_INPUT_K;
            } else if (!validMetric && !validK){
                answer += INVALID_INPUT_METRIC + "\n" + INVALID_INPUT_K;
            } else {
                answer += "valid";
                //set the new k to be newKValue
                cli->k = newKValue;
            }
        }
        sendToClient(answer);
        sleep(0.5);
    }
}