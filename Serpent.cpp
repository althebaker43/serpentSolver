
#include "Serpent.h"
#include "Block.h"
#include "Space.h"
#include "Coordinates.h"
#include <algorithm>


Serpent::Axis Serpent::OUR_ROT_AXES[3] = {AXIS_X, AXIS_Y, AXIS_Z};


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

    serpent->markEdgeBlocks();

    return serpent;
}


Serpent::Serpent() :
    myHead(new Block()),
    mySpace(NULL),
    myCurrentRotAxisIdx(0)
{
}

Serpent::~Serpent()
{
    closeStepFile();
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
            myXPos(0),
            myYPos(0),
            myZPos(0)
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

Coordinates*
Serpent::getBlocksPos()
{
    std::vector<ssize_t> xVals;
    std::vector<ssize_t> yVals;
    std::vector<ssize_t> zVals;

    getBlocksPos(xVals, yVals, zVals);

    size_t size = xVals.size();
    Coordinates* coordinates = new Coordinates();

    for(
            size_t idx = 0;
            idx < size;
            ++idx
       )
    {
        coordinates->add(
                xVals[idx],
                yVals[idx],
                zVals[idx]
                );
    }

    return coordinates;
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

size_t
Serpent::getVolume()
{
    size_t xSize = 0;
    size_t ySize = 0;
    size_t zSize = 0;

    getDimensions(
            xSize,
            ySize,
            zSize
            );

    return (xSize * ySize * zSize);
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
    return (hiBound - loBound + 1);
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

        bool myPreviousIsPivot;

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
            myCenter(center),
            myPreviousIsPivot(false)
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
                myPreviousIsPivot = false;
                return;
            }

            if (myPreviousIsPivot == true)
            {
                return;
            }

            if (block->isEdge() == true)
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

            float distance = abs(pos - myCenter);

            myPivotsDistances.push_back(
                    std::pair<Block*, float>(
                        block,
                        distance
                        )
                    );
            myPreviousIsPivot = true;
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

static bool
CheckCompression(
        bool    isSerpentValid,
        size_t  origVolume,
        size_t  newVolume,
        bool    isVolumeIgnored
        )
{
    if (isSerpentValid == false)
    {
        return false;
    }

    if (isVolumeIgnored == true)
    {
        return true;
    }

    if (newVolume < origVolume)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
Serpent::compress()
{
    Axes compressAxes;
    getMaxSizeAxes(compressAxes);

    size_t origVolume = getVolume();

    for(
            size_t attemptIdx = 0;
            attemptIdx < 2;
            ++attemptIdx
       )
    {
        bool isVolumeIgnored = (attemptIdx == 1);

        size_t origRotAxisIdx = myCurrentRotAxisIdx;
        myCurrentRotAxisIdx = (myCurrentRotAxisIdx < 2) ? myCurrentRotAxisIdx + 1 : 0;

        for(
                Axes::const_iterator compressAxisIter = compressAxes.begin();
                compressAxisIter != compressAxes.end();
                ++compressAxisIter
           )
        {
            Axis compressAxis = *compressAxisIter;

            for(
                    ;
                    myCurrentRotAxisIdx != origRotAxisIdx;
                    myCurrentRotAxisIdx = (myCurrentRotAxisIdx < 2) ? myCurrentRotAxisIdx + 1 : 0
               )
            {
                Axis rotAxis = OUR_ROT_AXES[myCurrentRotAxisIdx];
                if (rotAxis == compressAxis)
                {
                    continue;
                }

                Blocks pivots;
                getPivots(rotAxis, pivots);

                for(
                        Blocks::const_iterator pivotIter = pivots.begin();
                        pivotIter != pivots.end();
                        ++pivotIter
                   )
                {
                    Block* pivot = *pivotIter;

                    Rotation cwRot;
                    Rotation ccwRot;

                    switch (rotAxis)
                    {
                        case AXIS_X:
                            cwRot = ROT_X_CW;
                            ccwRot = ROT_X_CCW;
                            break;

                        case AXIS_Y:
                            cwRot = ROT_Y_CW;
                            ccwRot = ROT_Y_CCW;
                            break;

                        case AXIS_Z:
                            cwRot = ROT_Z_CW;
                            ccwRot = ROT_Z_CCW;
                            break;

                        default:
                            break;
                    };

                    pivot->rotate(cwRot);
                    if (CheckCompression(check(), origVolume, getVolume(), isVolumeIgnored))
                    {
                        mySteps.push_back(Step(pivot->getID(), cwRot));
                        return true;
                    }
                    else
                    {
                        pivot->rotate(ccwRot);
                    }

                    pivot->rotate(ccwRot);
                    if (CheckCompression(check(), origVolume, getVolume(), isVolumeIgnored))
                    {
                        mySteps.push_back(Step(pivot->getID(), ccwRot));
                        return true;
                    }
                    else
                    {
                        pivot->rotate(cwRot);
                    }
                }
            }
        }
    }

    return false;
}

class Serpent::EdgeBlockCollector : public Serpent::BlockIterator
{
    private:

        Blocks myHeadBlocks;

        bool myAtHead;

        Blocks myTailBlocks;

        Direction myPrevDir;

    public:

        EdgeBlockCollector() :
            myAtHead(true),
            myPrevDir(DIR_UP)
        {
        }

        void processHead(
                Block* head
                )
        {
            myHeadBlocks.push_back(head);

            head->getNext(myPrevDir);
        }

        void processBlock(
                Block*      block,
                Direction   dir
                )
        {
            if (dir == myPrevDir)
            {
                myTailBlocks.push_back(block);

                if (myAtHead == true)
                {
                    myHeadBlocks.push_back(block);
                }
            }
            else
            {
                myAtHead = false;
                myTailBlocks.clear();
            }
        }

        void getEdgeBlocks(
                Blocks& edgeBlocks
                )
        {
            edgeBlocks.clear();

            edgeBlocks.insert(
                    edgeBlocks.end(),
                    myHeadBlocks.begin(),
                    myHeadBlocks.end()
                    );
            edgeBlocks.insert(
                    edgeBlocks.end(),
                    myTailBlocks.begin(),
                    myTailBlocks.end()
                    );
        }
};

void
Serpent::markEdgeBlocks()
{
    Blocks edgeBlocks;

    EdgeBlockCollector collector;
    iterateOverBlocks(&collector);
    collector.getEdgeBlocks(edgeBlocks);

    for(
            Blocks::const_iterator blockIter = edgeBlocks.begin();
            blockIter != edgeBlocks.end();
            ++blockIter
       )
    {
        Block* block = *blockIter;

        block->setEdge(true);
    }
}

void
Serpent::writeSteps(
        std::ostream& outputStream
        ) const
{
    for(
            Steps::const_iterator stepIter = mySteps.begin();
            stepIter != mySteps.end();
            ++stepIter
       )
    {
        outputStream << stepIter->myBlockID << ' ';

        switch (stepIter->myRotation)
        {
            case ROT_X_CW:  outputStream << "x_cw";  break;
            case ROT_X_CCW: outputStream << "x_ccw"; break;
            case ROT_Y_CW:  outputStream << "y_cw";  break;
            case ROT_Y_CCW: outputStream << "y_ccw"; break;
            case ROT_Z_CW:  outputStream << "z_cw";  break;
            case ROT_Z_CCW: outputStream << "z_ccw"; break;
            default: break;
        };

        outputStream << std::endl;
    }
}

void
Serpent::openStepFile(
        const char* filename
        )
{
    if (myStepStream.is_open() == false)
    {
        myStepStream.open(filename);
    }
}

bool
Serpent::step()
{
    if (myStepStream.is_open() == false)
    {
        return false;
    }

    size_t blockID;
    std::string rotString;

    myStepStream >> blockID;
    myStepStream >> rotString;

    if (myStepStream.good() == false)
    {
        return false;
    }

    Block* block = getHead();
    for(
            ;
            block != NULL;
            block = block->getNext()
       )
    {
        blockID--;
        if (blockID == 0)
        {
            break;
        }
    }
    if (block == NULL)
    {
        return false;
    }

    Rotation rot;
    switch (rotString[0])
    {
        case 'x': rot = ((rotString.substr(3) == "cw") ? ROT_X_CW : ROT_X_CCW); break;
        case 'y': rot = ((rotString.substr(3) == "cw") ? ROT_Y_CW : ROT_Y_CCW); break;
        case 'z': rot = ((rotString.substr(3) == "cw") ? ROT_Z_CW : ROT_Z_CCW); break;
        default: return false; break;
    };

    block->rotate(rot);

    return true;
}

void
Serpent::closeStepFile()
{
    myStepStream.close();
}

