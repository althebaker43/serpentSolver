
#include "Space.h"


Space::Space(
        size_t  radius
        ) :
    myValues(NULL),
    myRadius(radius)
{
    size_t spaceSize = 2 * myRadius;

    myValues = new size_t** [spaceSize];

    for(
            size_t xPos = 0;
            xPos < spaceSize;
            ++xPos
       )
    {
        myValues[xPos] = new size_t* [spaceSize];

        for(
                size_t yPos = 0;
                yPos < spaceSize;
                ++yPos
           )
        {
            myValues[xPos][yPos] = new size_t [spaceSize];

            for(
                    size_t zPos = 0;
                    zPos < spaceSize;
                    ++zPos
               )
            {
                myValues[xPos][yPos][zPos] = 0;
            }
        }
    }
}

Space::~Space()
{
    size_t spaceSize = 2 * myRadius;

    for(
            size_t xPos = 0;
            xPos < spaceSize;
            ++xPos
       )
    {
        for(
                size_t yPos = 0;
                yPos < spaceSize;
                ++yPos
           )
        {
            delete [] myValues[xPos][yPos];
        }

        delete [] myValues[xPos];
    }

    delete [] myValues;

    myRadius = 0;
}

size_t
Space::get(
        ssize_t xPos,
        ssize_t yPos,
        ssize_t zPos
        ) const
{
    size_t xIdx = xPos + myRadius;
    size_t yIdx = yPos + myRadius;
    size_t zIdx = zPos + myRadius;

    return myValues[xIdx][yIdx][zIdx];
}

void
Space::set(
        ssize_t xPos,
        ssize_t yPos,
        ssize_t zPos,
        size_t val
        )
{
    size_t xIdx = xPos + myRadius;
    size_t yIdx = yPos + myRadius;
    size_t zIdx = zPos + myRadius;

    myValues[xIdx][yIdx][zIdx] = val;
}

void
Space::clear()
{
    size_t spaceSize = 2 * myRadius;

    for(
            size_t xPos = 0;
            xPos < spaceSize;
            ++xPos
       )
    {
        for(
                size_t yPos = 0;
                yPos < spaceSize;
                ++yPos
           )
        {
            for(
                    size_t zPos = 0;
                    zPos < spaceSize;
                    ++zPos
               )
            {
                myValues[xPos][yPos][zPos] = 0;
            }
        }
    }
}

