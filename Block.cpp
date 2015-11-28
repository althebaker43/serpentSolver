
#include "Block.h"
#include <stdlib.h>


Block::Block() :
    myNext(NULL),
    myNextDir(DIR_NONE)
{
}

Block::~Block()
{
    delete myNext;
}

Block*
Block::addNext(
        Block::Direction dir
        )
{
    if (myNext == NULL)
    {
        myNext = new Block();
        myNextDir = dir;
    }

    return myNext;
}

Block*
Block::getNext(
        Block::Direction& dir
        ) const
{
    dir = myNextDir;
    return myNext;
}

