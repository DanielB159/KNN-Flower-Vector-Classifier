
#ifndef TYPE_DISTANCES_H
#define TYPE_DISTANCES_H

#include <vector>
//this is an abstract class of a generic Distance. everhy class that inherits this class implements a functor distance function
class Distance{
    public:
        virtual double operator()(const std::vector<double>, const std::vector<double>) = 0;
        virtual ~Distance() { }
};

class ChebyshevDist: public Distance {
    public:
        double operator()(const std::vector<double>, const std::vector<double>);
        ChebyshevDist() { }
        ~ChebyshevDist() { }
};

class CanberraDist: public Distance {
    public:
        double operator()(const std::vector<double>, const std::vector<double>);
        CanberraDist() { }
        ~CanberraDist() { }
};

class MinkowskiDist: public Distance {
    public:
        double operator()(const std::vector<double>, const std::vector<double>, int p);
        double operator()(const std::vector<double>, const std::vector<double>);
        MinkowskiDist() { }
        ~MinkowskiDist() { }
};

class ManhattanDist: public Distance {
    public:
        double operator()(const std::vector<double>, const std::vector<double>);
        ManhattanDist() { }
        ~ManhattanDist() { }
};

class EuclidianDist: public Distance {
    public:
        double operator()(const std::vector<double>, const std::vector<double>);
        EuclidianDist() { }
        ~EuclidianDist() { }
};

#endif