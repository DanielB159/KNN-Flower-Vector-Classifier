#ifndef VECTOR_FEATURES
#define VECTOR_FEATURES
#include <vector>
#include <string>

//this nemaspace defines two structs of ways to save a vector. {vector, type} or {distanceFromInput, type}
namespace vectorFeatures{
    struct vectorType{
        std::vector<double> vect;
        std::string type;
    };

    struct distanceType{
        double distance;
        std::string type;
    };
}

#endif