#ifndef SPACE_H
#define SPACE_H

#include <stdlib.h>

class Space
{
    public:

        Space(
                size_t radius
             );

        ~Space();

        size_t get(
                ssize_t xPos,
                ssize_t yPos,
                ssize_t zPos
                ) const;

        void set(
                ssize_t xPos,
                ssize_t yPos,
                ssize_t zPos,
                size_t val
                );

        void clear();

    private:

        size_t*** myValues;

        size_t myRadius;
};

#endif
