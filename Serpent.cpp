
#include "Serpent.h"
#include "Block.h"
#include <fstream>
#include <algorithm>


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
            case 'u': curBlock = curBlock->addNext(DIR_UP); break;
            case 'd': curBlock = curBlock->addNext(DIR_DOWN); break;
            case 'f': curBlock = curBlock->addNext(DIR_FORWARD); break;
            case 'b': curBlock = curBlock->addNext(DIR_BACKWARD); break;
            case 'r': curBlock = curBlock->addNext(DIR_RIGHT); break;
            case 'l': curBlock = curBlock->addNext(DIR_LEFT); break;
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
Serpent::iterateOverBlocks(
        BlockIterator* iter
        )
{
    Block* curBlock = getHead();

    iter->processHead(curBlock);

    while (true)
    {
        Direction nextDir;
        curBlock = curBlock->getNext(nextDir);
        if (curBlock == NULL)
        {
            break;
        }

        iter->processBlock(curBlock, nextDir);
    }
}

class Serpent::PositionIterator : public Serpent::BlockIterator
{
    private:

        ssize_t myXPos;
        ssize_t myYPos;
        ssize_t myZPos;

    public:

        PositionIterator() :
            myXPos(1),
            myYPos(1),
            myZPos(1)
        {
        }

        virtual ~PositionIterator(){}

        void processBlock(
                Block*      block,
                Direction   dir
                )
        {
            switch (dir)
            {
                case DIR_UP:        ++myZPos; break;
                case DIR_DOWN:      --myZPos; break;
                case DIR_FORWARD:   ++myXPos; break;
                case DIR_BACKWARD:  --myXPos; break;
                case DIR_RIGHT:     --myYPos; break;
                case DIR_LEFT:      ++myYPos; break;
                default: break;
            };

            processPositionBlock(block, dir);
        }

    protected:

        virtual void processPositionBlock(
                Block*      block,
                Direction   dir
                ) = 0;

        ssize_t getXPos() const
        {
            return myXPos;
        }

        ssize_t getYPos() const
        {
            return myYPos;
        }

        ssize_t getZPos() const
        {
            return myZPos;
        }
};

class Serpent::TailPosCalculator : public Serpent::PositionIterator
{

    public:

        void processHead(
                Block* head
                )
        {
        }

        void processPositionBlock(
                Block*      block,
                Direction   dir
                )
        {
        }

        void getTailPos(
                ssize_t&    xPos,
                ssize_t&    yPos,
                ssize_t&    zPos
                ) const
        {
            xPos = getXPos();
            yPos = getYPos();
            zPos = getZPos();
        }
};

void
Serpent::getTailPos(
        ssize_t& xPos,
        ssize_t& yPos,
        ssize_t& zPos
        )
{
    TailPosCalculator calculator;
    iterateOverBlocks(&calculator);
    calculator.getTailPos(xPos, yPos, zPos);
}

void
Serpent::getDimensions(
        size_t& xSize,
        size_t& ySize,
        size_t& zSize
        )
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

class Serpent::BoundsCalculator : public Serpent::PositionIterator
{
    private:

        ssize_t myXMin;
        ssize_t myYMin;
        ssize_t myZMin;

        ssize_t myXMax;
        ssize_t myYMax;
        ssize_t myZMax;

        static ssize_t GetMax(
                ssize_t val1,
                ssize_t val2
                )
        {
            return ((val1 > val2) ? val1 : val2);
        }

        static ssize_t GetMin(
                ssize_t val1,
                ssize_t val2
                )
        {
            return ((val1 < val2) ? val1 : val2);
        }

    public:

        BoundsCalculator() :
            myXMin(0),
            myYMin(0),
            myZMin(0),
            myXMax(0),
            myYMax(0),
            myZMax(0)
        {
        }

        void processHead(
                Block*
                )
        {
        }

        void processPositionBlock(
                Block*      block,
                Direction   dir
                )
        {
            switch (dir)
            {
                case DIR_UP:        myZMax = GetMax(myZMax, getZPos()); break;
                case DIR_DOWN:      myZMin = GetMin(myZMin, getZPos()); break;
                case DIR_FORWARD:   myXMax = GetMax(myXMax, getXPos()); break;
                case DIR_BACKWARD:  myXMin = GetMin(myXMin, getXPos()); break;
                case DIR_RIGHT:     myYMin = GetMin(myYMin, getYPos()); break;
                case DIR_LEFT:      myYMax = GetMax(myYMax, getYPos()); break;
                default: break;
            };
        }

        void getBounds(
                ssize_t& xMin,
                ssize_t& yMin,
                ssize_t& zMin,
                ssize_t& xMax,
                ssize_t& yMax,
                ssize_t& zMax
                ) const
        {
            xMin = myXMin;
            yMin = myYMin;
            zMin = myZMin;
            xMax = myXMax;
            yMax = myYMax;
            zMax = myZMax;
        }
};

void
Serpent::getBounds(
        ssize_t& xMin,
        ssize_t& yMin,
        ssize_t& zMin,
        ssize_t& xMax,
        ssize_t& yMax,
        ssize_t& zMax
        )
{
    BoundsCalculator calculator;
    iterateOverBlocks(&calculator);
    calculator.getBounds(xMin, yMin, zMin, xMax, yMax, zMax);
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
        Direction nextDir;
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
            case DIR_UP: ++zPos; break;
            case DIR_DOWN: --zPos; break;
            case DIR_FORWARD: ++xPos; break;
            case DIR_BACKWARD: --xPos; break;
            case DIR_RIGHT: --yPos; break;
            case DIR_LEFT: ++yPos; break;
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

static bool
CompareSizePtrs(
        size_t* ptr1,
        size_t* ptr2
        )
{
    return ((*ptr1) > (*ptr2));
}

void
Serpent::getMaxSizeAxes(
        Serpent::Axes&  axes
        )
{
    axes.clear();

    size_t xSize = 0;
    size_t ySize = 0;
    size_t zSize = 0;

    getDimensions(xSize, ySize, zSize);

    std::vector<size_t*> sizes(3);
    sizes[0] = &xSize;
    sizes[1] = &ySize;
    sizes[2] = &zSize;

    std::sort(
            sizes.begin(),
            sizes.end(),
            CompareSizePtrs
            );

    for(
            size_t sizeIdx = 0;
            sizeIdx < 3;
            ++sizeIdx
       )
    {
        if (sizes[sizeIdx] == &xSize)
        {
            axes.push_back(AXIS_X);
        }
        else if (sizes[sizeIdx] == &ySize)
        {
            axes.push_back(AXIS_Y);
        }
        else
        {
            axes.push_back(AXIS_Z);
        }
    }
}

