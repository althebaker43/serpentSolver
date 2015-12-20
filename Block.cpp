
#include "Block.h"
#include <stdlib.h>


size_t Block::ourNextID = 1;


Direction Block::OUR_ROT_DIR_MAP[NUM_ROTS][NUM_DIRS] = {
    { DIR_RIGHT,    DIR_LEFT,     DIR_FORWARD, DIR_BACKWARD, DIR_DOWN,     DIR_UP       },
    { DIR_LEFT,     DIR_RIGHT,    DIR_FORWARD, DIR_BACKWARD, DIR_UP,       DIR_DOWN     },
    { DIR_FORWARD,  DIR_BACKWARD, DIR_DOWN,    DIR_UP,       DIR_RIGHT,    DIR_LEFT     },
    { DIR_BACKWARD, DIR_FORWARD,  DIR_UP,      DIR_DOWN,     DIR_RIGHT,    DIR_LEFT     },
    { DIR_UP,       DIR_DOWN,     DIR_LEFT,    DIR_RIGHT,    DIR_FORWARD,  DIR_BACKWARD },
    { DIR_UP,       DIR_DOWN,     DIR_RIGHT,   DIR_LEFT,     DIR_BACKWARD, DIR_FORWARD  }
};

Block::Block() :
    myNext(NULL),
    myNextDir(DIR_UP),
    MY_ID(ourNextID++),
    myIsEdge(false)
{
}

Block::~Block()
{
    delete myNext;
}

Block*
Block::addNext(
        Direction dir
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
        Direction& dir
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

size_t
Block::getID() const
{
    return MY_ID;
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

void
Block::setEdge(
        bool isEdge
        )
{
    myIsEdge = isEdge;
}

bool
Block::isEdge() const
{
    return myIsEdge;
}

