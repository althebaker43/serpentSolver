
#include "Serpent.h"
#include "Block.h"
#include <fstream>


Serpent*
Serpent::CreateFromXML(
        const char* filename
        )
{
    return NULL;
}

Serpent*
Serpent::CreateFromPath(
        const char* filename
        )
{
    std::ifstream pathStream(filename);
    std::string curLine;
    Serpent* serpent = new Serpent();
    Block* curBlock = serpent->getHead();

    while (pathStream.good() == true)
    {
        std::getline(pathStream, curLine);

        if (curLine.length() < 1)
        {
            continue;
        }

        switch (curLine[0])
        {
            case 'u': curBlock = curBlock->addNext(Block::DIR_UP); break;
            case 'd': curBlock = curBlock->addNext(Block::DIR_DOWN); break;
            case 'f': curBlock = curBlock->addNext(Block::DIR_FORWARD); break;
            case 'b': curBlock = curBlock->addNext(Block::DIR_BACKWARD); break;
            case 'r': curBlock = curBlock->addNext(Block::DIR_RIGHT); break;
            case 'l': curBlock = curBlock->addNext(Block::DIR_LEFT); break;
            default: break;
        };
    }

    pathStream.close();

    return serpent;
}


Serpent::Serpent() :
    myHead(new Block()),
    mySpace(NULL),
    mySpaceRadius(0)
{
}

Serpent::~Serpent()
{
    delete myHead;
    deleteSpace();
}

Block*
Serpent::getHead() const
{
    return myHead;
}

void
Serpent::getTailPos(
        ssize_t& xPos,
        ssize_t& yPos,
        ssize_t& zPos
        ) const
{
    Block* curBlock = getHead();
    xPos = 1;
    yPos = 1;
    zPos = 1;

    while (true)
    {
        Block::Direction nextDir;
        curBlock = curBlock->getNext(nextDir);
        if (curBlock == NULL)
        {
            break;
        }

        switch (nextDir)
        {
            case Block::DIR_UP: ++zPos; break;
            case Block::DIR_DOWN: --zPos; break;
            case Block::DIR_FORWARD: ++xPos; break;
            case Block::DIR_BACKWARD: --xPos; break;
            case Block::DIR_RIGHT: --yPos; break;
            case Block::DIR_LEFT: ++yPos; break;
            default: break;
        };
    }
}

void
Serpent::getDimensions(
        size_t& xSize,
        size_t& ySize,
        size_t& zSize
        ) const
{
    ssize_t xMin = 0;
    ssize_t yMin = 0;
    ssize_t zMin = 0;
    ssize_t xMax = 0;
    ssize_t yMax = 0;
    ssize_t zMax = 0;

    getBounds(
            xMin,
            yMin,
            zMin,
            xMax,
            yMax,
            zMax
            );

    xSize = GetLength(xMax, xMin);
    ySize = GetLength(yMax, yMin);
    zSize = GetLength(zMax, zMin);
}

void
Serpent::getBounds(
        ssize_t& xMin,
        ssize_t& yMin,
        ssize_t& zMin,
        ssize_t& xMax,
        ssize_t& yMax,
        ssize_t& zMax
        ) const
{
    Block* curBlock = getHead();
    ssize_t xPos = 1;
    ssize_t yPos = 1;
    ssize_t zPos = 1;

    xMin = 0;
    yMin = 0;
    zMin = 0;
    xMax = 0;
    yMax = 0;
    zMax = 0;

    while (true)
    {
        Block::Direction nextDir;
        curBlock = curBlock->getNext(nextDir);
        if (curBlock == NULL)
        {
            break;
        }

        switch (nextDir)
        {
            case Block::DIR_UP: zMax = GetMax(zMax, ++zPos); break;
            case Block::DIR_DOWN: zMin = GetMin(zMin, --zPos); break;
            case Block::DIR_FORWARD: xMax = GetMax(xMax, ++xPos); break;
            case Block::DIR_BACKWARD: xMin = GetMin(xMin, --xPos); break;
            case Block::DIR_RIGHT: yMin = GetMin(yMin, --yPos); break;
            case Block::DIR_LEFT: yMax = GetMax(yMax, ++yPos); break;
            default: break;
        };
    }
}

ssize_t
Serpent::GetMax(
        ssize_t val1,
        ssize_t val2
        )
{
    return ((val1 > val2) ? val1 : val2);
}

ssize_t
Serpent::GetMin(
        ssize_t val1,
        ssize_t val2
        )
{
    return ((val1 < val2) ? val1 : val2);
}

size_t
Serpent::GetLength(
        ssize_t hiBound,
        ssize_t loBound
        )
{
    size_t length = hiBound - loBound;

    if (length > 0)
    {
        return length;
    }
    else
    {
        return 1;
    }
}

bool
Serpent::check()
{
    if (mySpace == NULL)
    {
        initializeSpace();
    }
    else
    {
        clearSpace();
    }

    Block* curBlock = getHead();
    ssize_t xPos = 1;
    ssize_t yPos = 1;
    ssize_t zPos = 1;

    while (true)
    {
        Block::Direction nextDir;
        curBlock = curBlock->getNext(nextDir);
        if (curBlock == NULL)
        {
            break;
        }

        if (getSpace(xPos, yPos, zPos) != 0)
        {
            return false;
        }

        setSpace(xPos, yPos, zPos, 1);

        switch (nextDir)
        {
            case Block::DIR_UP: ++zPos; break;
            case Block::DIR_DOWN: --zPos; break;
            case Block::DIR_FORWARD: ++xPos; break;
            case Block::DIR_BACKWARD: --xPos; break;
            case Block::DIR_RIGHT: --yPos; break;
            case Block::DIR_LEFT: ++yPos; break;
            default: break;
        };
    }

    return true;
}

void
Serpent::initializeSpace()
{
    mySpaceRadius = getNumBlocks();
    size_t spaceSize = 2 * mySpaceRadius;

    mySpace = new size_t** [spaceSize];

    for(
            size_t xPos = 0;
            xPos < spaceSize;
            ++xPos
       )
    {
        mySpace[xPos] = new size_t* [spaceSize];

        for(
                size_t yPos = 0;
                yPos < spaceSize;
                ++yPos
           )
        {
            mySpace[xPos][yPos] = new size_t [spaceSize];

            for(
                    size_t zPos = 0;
                    zPos < spaceSize;
                    ++zPos
               )
            {
                mySpace[xPos][yPos][zPos] = 0;
            }
        }
    }
}

void
Serpent::deleteSpace()
{
    size_t spaceSize = 2 * mySpaceRadius;

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
            delete [] mySpace[xPos][yPos];
        }

        delete [] mySpace[xPos];
    }

    delete [] mySpace;

    mySpaceRadius = 0;
}

size_t
Serpent::getNumBlocks() const
{
    size_t numBlocks = 0;

    for(
            Block* curBlock = getHead();
            curBlock != NULL;
            curBlock = curBlock->getNext()
       )
    {
        ++numBlocks;
    }

    return numBlocks;
}

size_t
Serpent::getSpace(
        ssize_t xPos,
        ssize_t yPos,
        ssize_t zPos
        ) const
{
    size_t xIdx = xPos + mySpaceRadius;
    size_t yIdx = yPos + mySpaceRadius;
    size_t zIdx = zPos + mySpaceRadius;

    return mySpace[xIdx][yIdx][zIdx];
}

void
Serpent::setSpace(
        ssize_t xPos,
        ssize_t yPos,
        ssize_t zPos,
        size_t val
        )
{
    size_t xIdx = xPos + mySpaceRadius;
    size_t yIdx = yPos + mySpaceRadius;
    size_t zIdx = zPos + mySpaceRadius;

    mySpace[xIdx][yIdx][zIdx] = val;
}

void
Serpent::clearSpace()
{
    size_t spaceSize = 2 * mySpaceRadius;

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
                mySpace[xPos][yPos][zPos] = 0;
            }
        }
    }
}
