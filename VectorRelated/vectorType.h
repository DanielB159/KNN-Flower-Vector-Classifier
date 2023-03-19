#ifndef VECTOR_TYPE_H
#define VECTOR_TYPE_H

#include "vectorFeatures.h"
#include <map>

namespace classificationOfVec{
    std::string getVecType(const vectorFeatures::distanceType* vecArr, int k);
}

#endif