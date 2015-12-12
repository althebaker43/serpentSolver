
#include "Coordinates.h"


void
Coordinates::add(
        ssize_t xVal,
        ssize_t yVal,
        ssize_t zVal
        )
{
    myXVals.push_back(xVal);
    myYVals.push_back(yVal);
    myZVals.push_back(zVal);
}

size_t
Coordinates::getSize() const
{
    return myXVals.size();
}

int
Coordinates::getXVal(
        size_t idx
        ) const
{
    return myXVals[idx];
}

int
Coordinates::getYVal(
        size_t idx
        ) const
{
    return myYVals[idx];
}

int
Coordinates::getZVal(
        size_t idx
        ) const
{
    return myZVals[idx];
}
