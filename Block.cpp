
#include "Block.h"
#include <stdlib.h>


Block::Direction Block::OUR_ROT_DIR_MAP[Block::NUM_ROTS][Block::NUM_DIRS] = {
    { Block::DIR_RIGHT,    Block::DIR_LEFT,     Block::DIR_FORWARD, Block::DIR_BACKWARD, Block::DIR_DOWN,     Block::DIR_UP       },
    { Block::DIR_LEFT,     Block::DIR_RIGHT,    Block::DIR_FORWARD, Block::DIR_BACKWARD, Block::DIR_UP,       Block::DIR_DOWN     },
    { Block::DIR_FORWARD,  Block::DIR_BACKWARD, Block::DIR_DOWN,    Block::DIR_UP,       Block::DIR_RIGHT,    Block::DIR_LEFT     },
    { Block::DIR_BACKWARD, Block::DIR_FORWARD,  Block::DIR_UP,      Block::DIR_DOWN,     Block::DIR_RIGHT,    Block::DIR_LEFT     },
    { Block::DIR_UP,       Block::DIR_DOWN,     Block::DIR_LEFT,    Block::DIR_RIGHT,    Block::DIR_FORWARD,  Block::DIR_BACKWARD },
    { Block::DIR_UP,       Block::DIR_DOWN,     Block::DIR_RIGHT,   Block::DIR_LEFT,     Block::DIR_BACKWARD, Block::DIR_FORWARD  }
};

Block::Block() :
    myNext(NULL),
    myNextDir(DIR_UP)
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

Block*
Block::getNext() const
{
    return myNext;
}

void
Block::rotate(
        Rotation rot
        )
{
    myNextDir = OUR_ROT_DIR_MAP[rot][myNextDir];

    if (myNext != NULL)
    {
        myNext->rotate(rot);
    }
}

