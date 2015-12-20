#ifndef BLOCK_H
#define BLOCK_H

#include "Common.h"
#include <stdlib.h>

class Block
{
    public:

        Block();

        ~Block();

        Block* addNext(
                Direction dir
                );

        Block* getNext(
                Direction& dir
                ) const;

        Block* getNext() const;

        size_t getID() const;

        void setEdge(
                bool isEdge
                );

        bool isEdge() const;

        void rotate(
                Rotation rot
                );

    private:

        static Direction OUR_ROT_DIR_MAP [NUM_ROTS] [NUM_DIRS];

        static size_t ourNextID;

        Block* myNext;

        Direction myNextDir;

        const size_t MY_ID;

        bool myIsEdge;
};

#endif
