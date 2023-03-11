#include "typeDistances.h"
#include <iostream>
#include <cmath>


/// @brief Calculate the Canberra Distance between two vectors.
/// @param v1 First vector
/// @param v2 Second vector
/// @return Canberra distance
double CanberraDist::operator()(const std::vector<double> v1, const std::vector<double> v2) {
    //make sure that the vectors are of the same size.
    if (v1.size() != v2.size()) {
        std::cout << "Error: Vector sizes not equal, returning -1." << std::endl;
        return -1;
    }
    //get the dimention of the vectors.
    int dim = v1.size();
    double sum = 0;
    //looping over the elements of the vectors, adding them corresponding to the distance formula.
    for (int i = 0; i < dim; i++) {
        double denominator = std::abs(v1[i]) + std::abs(v2[i]);
        //making sure not to divide by zero in the calculation of the distance.
        sum += (denominator == 0 ? 0 : (std::abs(v1[i] - v2[i]))/denominator);
    }
    return sum;
}

/// @brief Calculate the Euclidian Distance between two vectors.
/// @param v1 First vector
/// @param v2 Second vector
/// @return Euclidian distance
double EuclidianDist::operator()(const std::vector<double> v1, const std::vector<double> v2) {
    MinkowskiDist m;
    return m(v1, v2, 2);
}

/// @brief Calculate the Chebyshev Distance between two vectors.
/// @param v1 First vector
/// @param v2 Second vector
/// @return Chebyshev distance
double ChebyshevDist::operator()(const std::vector<double> v1, const std::vector<double> v2) {
    //make sure that the vectors are of the same size.
    if (v1.size() != v2.size()) {
        std::cout << "Error: Vector sizes not equal, returning -1." << std::endl;
        return -1;
    }
    //get the dimention of the vectors
    int dim = v1.size();
    //set the first value to be te max, then compare to all of the others.
    double max = std::abs(v1[0] - v2[0]);
    for (int i = 1; i < dim; i++) {
        double curr = std::abs(v1[i] - v2[i]);
        max = (curr > max ? curr : max);
    }
    return max;
}


/// @brief Calculate the Minkowski Distance between two vectors where p = 2.
/// @param v1 First vector
/// @param v2 Second vector
/// @param p Minkowski parameter, default value 2
/// @return Minkowski distance
double MinkowskiDist::operator()(const std::vector<double> v1, const std::vector<double> v2) {
    return this->operator()(v1,v2, 2);
}


/// @brief Calculate the minkowski Distance between two vectors.
/// @param v1 First vector
/// @param v2 Second vector
/// @return minkowski distance distance
double MinkowskiDist::operator()(const std::vector<double> v1, const std::vector<double> v2, int p) {
    //make sure that the vectors are of the same size.
    if (v1.size() != v2.size()) {
        std::cout << "Error: Vector sizes not equal, returning -1." << std::endl;
        return -1;
    }
    int dim = v1.size();

    double normalSum = 0;
    for (int i = 0; i < dim; i++) {
        //adding each sum of elements to the power of (1/p) to the sum.
        double d = std::abs(v1[i] - v2[i]);
        normalSum += std::pow(d, p);
    }
    return std::pow(normalSum, 1.0/p);
}

/// @brief Calculate the Manhatten Distance between two vectors.
/// @param v1 First vector
/// @param v2 Second vector
/// @return Manhatten distance
double ManhattanDist::operator()(const std::vector<double> v1, const std::vector<double> v2) {
    MinkowskiDist m;
    return m(v1, v2, 1);
}
