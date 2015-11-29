#ifndef BLOCK_H
#define BLOCK_H

class Block
{
    public:

        enum Direction
        {
            DIR_UP,
            DIR_DOWN,
            DIR_FORWARD,
            DIR_BACKWARD,
            DIR_RIGHT,
            DIR_LEFT,
            NUM_DIRS
        };

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

        void rotate(
                Rotation rot
                );

    private:

        static Direction OUR_ROT_DIR_MAP [NUM_ROTS] [NUM_DIRS];

        Block* myNext;

        Direction myNextDir;
};

#endif
