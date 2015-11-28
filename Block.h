#ifndef BLOCK_H
#define BLOCK_H

class Block
{
    public:

        enum Direction
        {
            DIR_NONE,
            DIR_UP,
            DIR_DOWN,
            DIR_FORWARD,
            DIR_BACKWARD,
            DIR_RIGHT,
            DIR_LEFT
        };

        Block();

        ~Block();

        Block* addNext(
                Direction dir
                );

        Block* getNext(
                Direction& dir
                ) const;

    private:

        Block* myNext;

        Direction myNextDir;
};

#endif
