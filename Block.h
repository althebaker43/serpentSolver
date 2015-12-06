#ifndef BLOCK_H
#define BLOCK_H

#include "Common.h"

class Block
{
    public:

        enum Rotation
        {
            ROT_X_CW,
            ROT_X_CCW,
            ROT_Y_CW,
            ROT_Y_CCW,
            ROT_Z_CW,
            ROT_Z_CCW,
            NUM_ROTS
        };

        Block();

        ~Block();

        Block* addNext(
                Direction dir
                );

        Block* getNext(
                Direction& dir
                ) const;

        Block* getNext() const;

        void setTail(
                bool isTail
                );

        bool isTail() const;

        void rotate(
                Rotation rot
                );

    private:

        static Direction OUR_ROT_DIR_MAP [NUM_ROTS] [NUM_DIRS];

        Block* myNext;

        Direction myNextDir;

        bool myIsTail;
};

#endif
