#include "ClassifyData.h"

ClassifyData::ClassifyData(int clientSocket) {
    setDescription("classify data");
    setDio(clientSocket);
};

/// @brief execute command
/// @param cliDetails details for execution
void ClassifyData::execute(CLIDetails* cli) {
    if (cli->classifiedVectors.size() == 0) {
        sendToClient("please upload data.");
        return;
    }
    //create comparator
    slct::Comparator<vectorFeatures::distanceType> comparator;
    //classify unclassified vectors
    for (int i = 0; i < cli->unclassifiedVectors.size(); i++) {
        vector<vectorFeatures::distanceType> classificationDistances;
        for (int j = 0; j < cli->classifiedVectors.size(); j++) {
            //get distances for all classified vectors and this current one.
            vectorFeatures::distanceType tempDist = {(*(cli->distanceMetricPtr))(cli->unclassifiedVectors[i].vect, cli->classifiedVectors[j].vect), cli->classifiedVectors[j].type};
            classificationDistances.push_back(tempDist);
        }

        //select and partition k closest elements
        vectorFeatures::distanceType* distanceArray = classificationDistances.data();
        slct::select(distanceArray, classificationDistances.size(), comparator, cli->k);
        //count occurances of a classification
        cli->unclassifiedVectors[i].type = classificationOfVec::getVecType(distanceArray, cli->k);
    }
    sendToClient("classifying data complete");
}