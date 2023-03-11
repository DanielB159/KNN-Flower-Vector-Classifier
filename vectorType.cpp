#include "vectorType.h"


/// @brief this function returns the most frequent type of vector in the first k elements of the distanceType array.
/// @param vecArr - an array of distanceType.
/// @param k - the index to check until, from index 0 to index k (assumed to be non-negative).
/// @return std::string - the most frequent type.
std::string classificationOfVec::getVecType(const vectorFeatures::distanceType* vecArr, int k) {
    //map will map from the vector type to the count of times it has appeared in the input array.
    std::map<std::string, int> map;
    //first, initializing all of the counters to be 0.
    for (int i = 0; i < k; i++) {
        map.insert(std::pair<std::string, int>(vecArr[i].type, 0));
    }
    //now, incrementing the counters in accordance to vecArr.
    for (int i = 0; i < k; i++) {
        map[vecArr[i].type]++;
    }
    int maxFrequency = 0;
    //finding the maximum frequency of some vector.
    for (int i = 0; i < k; i++) {
        int frequency = map[vecArr[i].type];
        if (frequency > maxFrequency) {
            maxFrequency = frequency;
        }
    }
    //returning the first vector of the maximum frequency.
    for (int i = 0; i < k; i++) {
        int frequency = map[vecArr[i].type];
        //because of the way maxFrequency is calculated, this if condition will be met at least once in this loop.
        if (frequency == maxFrequency) {
            return vecArr[i].type;
        }
    }
    //if the if condition was not met, there was an error in the calculation.
    return "Error while computing max vector!";
}