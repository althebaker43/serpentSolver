
#include "Serpent.h"
#include "Block.h"
#include "Space.h"
#include <fstream>
#include <algorithm>


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

    serpent->markTailBlocks();

    return serpent;
}


Serpent::Serpent() :
    myHead(new Block()),
    mySpace(NULL)
{
}

Serpent::~Serpent()
{
    delete myHead;
    delete mySpace;
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

class Serpent::BlocksPosCalculator : public Serpent::PositionIterator
{
    private:

        std::vector<ssize_t> myXPos;
        std::vector<ssize_t> myYPos;
        std::vector<ssize_t> myZPos;

    public:

        void processHead(
                Block* head
                )
        {
            myXPos.push_back(getXPos());
            myYPos.push_back(getYPos());
            myZPos.push_back(getZPos());
        }

        void processPositionBlock(
                Block*      block,
                Direction   dir
                )
        {
            myXPos.push_back(getXPos());
            myYPos.push_back(getYPos());
            myZPos.push_back(getZPos());
        }

        void getBlocksPos(
                std::vector<ssize_t>& xPos,
                std::vector<ssize_t>& yPos,
                std::vector<ssize_t>& zPos
                )
        {
            xPos = myXPos;
            yPos = myYPos;
            zPos = myZPos;
        }
};

void
Serpent::getBlocksPos(
        std::vector<ssize_t>& xPos,
        std::vector<ssize_t>& yPos,
        std::vector<ssize_t>& zPos
        )
{
    BlocksPosCalculator calculator;
    iterateOverBlocks(&calculator);
    calculator.getBlocksPos(xPos, yPos, zPos);
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

class Serpent::Checker : public Serpent::PositionIterator
{
    private:

        Space* mySpace;

        bool myIsValid;

    public:

        Checker(
                Space* space
               ) :
            mySpace(space),
            myIsValid(true)
        {
        }

        void processHead(
                Block* head
                )
        {
            mySpace->set(getXPos(), getYPos(), getZPos(), 1);
        }

        void processPositionBlock(
                Block*      block,
                Direction   dir
                )
        {
            if (mySpace->get(getXPos(), getYPos(), getZPos()) != 0)
            {
                myIsValid = false;
            }

            mySpace->set(getXPos(), getYPos(), getZPos(), 1);
        }

        bool isValid() const
        {
            return myIsValid;
        }
};

bool
Serpent::check()
{
    if (mySpace == NULL)
    {
        mySpace = new Space(getNumBlocks());
    }
    else
    {
        mySpace->clear();
    }

    Checker checker(mySpace);
    iterateOverBlocks(&checker);

    return checker.isValid();
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

static float
getCenter(
        ssize_t min,
        ssize_t max
        )
{
    return ((max - min) / 2.0);
}

class Serpent::PivotBlockCollector : public Serpent::PositionIterator
{
    private:

        const Axis myAxis;

        const float myCenter;

        /**
         * Container of pivots and their distances from the center
         */
        std::vector<std::pair<Block*, float> > myPivotsDistances;

        bool isPivot(
                Direction dir
                ) const
        {
            if(
                    (myAxis == AXIS_X) &&
                    (
                     (dir == DIR_FORWARD) ||
                     (dir == DIR_BACKWARD)
                    )
              )
            {
                return true;
            }
            else if(
                    (myAxis == AXIS_Y) &&
                    (
                     (dir == DIR_RIGHT) ||
                     (dir == DIR_LEFT)
                    )
                   )
            {
                return true;
            }
            else if(
                    (myAxis == AXIS_Z) &&
                    (
                     (dir == DIR_UP) ||
                     (dir == DIR_DOWN)
                    )
                   )
            {
                return true;
            }

            return false;
        }

        static bool ComparePivotsDistances(
                const std::pair<Block*, float>& pair1,
                const std::pair<Block*, float>& pair2
                )
        {
            return ((pair1.second) < (pair2.second));
        }

    public:

        PivotBlockCollector(
                Axis    axis,
                float   center
                ) :
            myAxis(axis),
            myCenter(center)
        {
        }

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
            if (isPivot(dir) == false)
            {
                return;
            }

            if (block->isTail() == true)
            {
                return;
            }

            ssize_t pos = 0;
            switch (myAxis)
            {
                case AXIS_X: pos = getXPos(); break;
                case AXIS_Y: pos = getYPos(); break;
                case AXIS_Z: pos = getZPos(); break;
                default: break;
            }

            float distance = pos - myCenter;

            myPivotsDistances.push_back(
                    std::pair<Block*, float>(
                        block,
                        distance
                        )
                    );
        }

        void getPivots(
                Blocks& pivots
                )
        {
            pivots.clear();

            std::sort(
                    myPivotsDistances.begin(),
                    myPivotsDistances.end(),
                    ComparePivotsDistances
                    );

            for(
                    std::vector<std::pair<Block*, float> >::const_iterator iter = myPivotsDistances.begin();
                    iter != myPivotsDistances.end();
                    ++iter
               )
            {
                pivots.push_back(iter->first);
            }
        }
};

void
Serpent::getPivots(
        Axis    axis,
        Blocks& blocks
        )
{
    blocks.clear();

    ssize_t xMin = 0;
    ssize_t yMin = 0;
    ssize_t zMin = 0;
    ssize_t xMax = 0;
    ssize_t yMax = 0;
    ssize_t zMax = 0;

    getBounds(xMin, yMin, zMin, xMax, yMax, zMax);

    float center = 0.0;
    switch (axis)
    {
        case AXIS_X: center = getCenter(xMin, xMax); break;
        case AXIS_Y: center = getCenter(yMin, yMax); break;
        case AXIS_Z: center = getCenter(zMin, zMax); break;
        default: break;
    };

    PivotBlockCollector collector(axis, center);
    iterateOverBlocks(&collector);
    collector.getPivots(blocks);
}

bool
Serpent::compress()
{
    Axes compressAxes;
    getMaxSizeAxes(compressAxes);

    for(
            Axes::const_iterator compressAxisIter = compressAxes.begin();
            compressAxisIter != compressAxes.end();
            ++compressAxisIter
       )
    {
        Axis compressAxis = *compressAxisIter;
        Axes rotAxes(2);

        switch (compressAxis)
        {
            case AXIS_X:
                rotAxes[0] = AXIS_Y;
                rotAxes[1] = AXIS_Z;
                break;

            case AXIS_Y:
                rotAxes[0] = AXIS_X;
                rotAxes[1] = AXIS_Z;
                break;

            case AXIS_Z:
                rotAxes[0] = AXIS_X;
                rotAxes[1] = AXIS_Y;
                break;

            default:
                break;
        };

        for(
                Axes::const_iterator rotAxisIter = rotAxes.begin();
                rotAxisIter != rotAxes.end();
                ++rotAxisIter
           )
        {
            Axis rotAxis = *rotAxisIter;

            Blocks pivots;
            getPivots(rotAxis, pivots);

            for(
                    Blocks::const_iterator pivotIter = pivots.begin();
                    pivotIter != pivots.end();
                    ++pivotIter
               )
            {
                Block* pivot = *pivotIter;

                Block::Rotation cwRot;
                Block::Rotation ccwRot;

                switch (rotAxis)
                {
                    case AXIS_X:
                        cwRot = Block::ROT_X_CW;
                        ccwRot = Block::ROT_X_CCW;
                        break;

                    case AXIS_Y:
                        cwRot = Block::ROT_Y_CW;
                        ccwRot = Block::ROT_Y_CCW;
                        break;

                    case AXIS_Z:
                        cwRot = Block::ROT_Z_CW;
                        ccwRot = Block::ROT_Z_CCW;
                        break;

                    default:
                        break;
                };

                pivot->rotate(cwRot);
                if (check() == true)
                {
                    return true;
                }
                else
                {
                    pivot->rotate(ccwRot);
                }

                pivot->rotate(ccwRot);
                if (check() == true)
                {
                    return true;
                }
                else
                {
                    pivot->rotate(cwRot);
                }
            }
        }
    }

    return false;
}

class Serpent::TailBlockCollector : public Serpent::BlockIterator
{
    private:

        Blocks myTailBlocks;

        Direction myPrevDir;

    public:

        TailBlockCollector() :
            myPrevDir(DIR_UP)
        {
        }

        void processHead(
                Block* head
                )
        {
        }

        void processBlock(
                Block*      block,
                Direction   dir
                )
        {
            if (dir == myPrevDir)
            {
                myTailBlocks.push_back(block);
            }
            else
            {
                myTailBlocks.clear();
            }
        }

        void getTailBlocks(
                Blocks& tailBlocks
                )
        {
            tailBlocks = myTailBlocks;
        }
};

void
Serpent::markTailBlocks()
{
    Blocks tailBlocks;

    TailBlockCollector collector;
    iterateOverBlocks(&collector);
    collector.getTailBlocks(tailBlocks);

    for(
            Blocks::const_iterator blockIter = tailBlocks.begin();
            blockIter != tailBlocks.end();
            ++blockIter
       )
    {
        Block* block = *blockIter;

        block->setTail(true);
    }
}

