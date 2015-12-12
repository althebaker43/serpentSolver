#ifndef COORDINATES_H
#define COORDINATES_H

#include <stdlib.h>
#include <vector>

class Coordinates
{
    public:

        void add(
                ssize_t xVal,
                ssize_t yVal,
                ssize_t zVal
           );

        size_t getSize() const;

        int getXVal(size_t idx) const;
        int getYVal(size_t idx) const;
        int getZVal(size_t idx) const;

    private:

        std::vector<ssize_t> myXVals;
        std::vector<ssize_t> myYVals;
        std::vector<ssize_t> myZVals;
};

#endif
