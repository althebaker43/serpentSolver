#ifndef SERPENT_H
#define SERPENT_H

#include "Common.h"
#include <ios>
#include <stdlib.h>
#include <vector>

class Block;
class BlockIterator;
class Space;

class Serpent
{
    public:

        enum Axis{
            AXIS_X,
            AXIS_Y,
            AXIS_Z
        };

        typedef std::vector<Axis> Axes;

        static Serpent* CreateFromPath(
                const char* filename
                );

        ~Serpent();

        Block* getHead() const;

        void getTailPos(
                ssize_t& xPos,
                ssize_t& yPos,
                ssize_t& zPos
                );

        void getDimensions(
                size_t& xSize,
                size_t& ySize,
                size_t& zSize
                );

        bool check();

        bool compress();

    private:

        class BlockIterator
        {
            public:

                virtual ~BlockIterator(){}

                virtual void processHead(
                        Block* head
                        ) = 0;

                virtual void processBlock(
                        Block*      block,
                        Direction   dir     /**< Direction traveled to get to block */
                        ) = 0;
        };

        class PositionIterator;
        class TailPosCalculator;
        class BoundsCalculator;
        class Checker;
        class PivotBlockCollector;
        class TailBlockCollector;

        typedef std::vector<Block*> Blocks;

        static ssize_t GetMax(
                ssize_t val1,
                ssize_t val2
                );

        static ssize_t GetMin(
                ssize_t val1,
                ssize_t val2
                );

        static size_t GetLength(
                ssize_t hiBound,
                ssize_t loBound
                );

        Serpent();

        void iterateOverBlocks(
                BlockIterator* iter
                );

        size_t getNumBlocks() const;

        void getBounds(
                ssize_t& xMin,
                ssize_t& yMin,
                ssize_t& zMin,
                ssize_t& xMax,
                ssize_t& yMax,
                ssize_t& zMax
                );

        void getMaxSizeAxes(
                Axes& axes  /**< Set of axes sorted by maximum size */
                );

        void getPivots(
                Axis    axis,   /**< Axis to look for center on */
                Blocks& blocks  /**< Pivot points sorted by distance from center */
                );

        void markTailBlocks();

        Block* myHead;

        Space* mySpace;
};

#endif
